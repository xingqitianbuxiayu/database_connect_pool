#include "connectionPool.h"
#include <fstream>
#include <thread>
#include <json/json.h> // 用于解析json,其头文件位于/usr/local/include/json中
using namespace Json;

ConnectionPool* ConnectionPool::getConnectionPool(string confFilePath)
{
    // 当第一次调用getConnectionPool时，该静态局部变量被创建，后续再次调用时，该变量已存在，访问的是同一块内存地址。
    // 静态局部变量不会随着函数返回而销毁，其生命周期为当应用程序结束之后，该内存才会被析构。
    // 静态局部变量的访问范围是该函数，只能在当前函数内被访问，可以选择返回该静态对象的地址。
    // 可以是静态局部变量，也可以是类的静态成员变量(指在头文件中定义)，这里我们使用静态局部变量
    // 这里创建了一个ConnectionPool类型的对象pool，调用ConnectionPool的默认构造函数
    static ConnectionPool pool(confFilePath);
    return &pool;
}

// 因为需要在构造函数ConnectionPool中使用，因此最好将conFilePath定义为类的一个私有变量成员
bool ConnectionPool::parseJsonFile(string confFilePath)
{
    // 定义一个读操作文件类的对象
    // 也可使用ifs的open()函数来打开文件
    // "dbconf.json"
    ifstream ifs(confFilePath);
    Reader rd; // jsoncpp提供的操作类
    Value root;
    rd.parse(ifs, root); // 解析ifs文件对象，并将结果解析赋值到root中
    if(root.isObject()) //存储到root中的是不是一个json对象（这里我们没用数组）
    {
        // 通过键获取对应的值，并通过Value类提供的方法转换为相应的数据类型
        m_ip = root["ip"].asString();
        m_user = root["userName"].asString();
        m_dbName = root["dbName"].asString();
        m_passwd = root["password"].asString();
        m_port = root["port"].asInt();
        m_minSize = root["minSize"].asInt();
        m_maxSize = root["maxSize"].asInt();
        m_maxIdleTime = root["maxIdleTime"].asInt();
        m_timeout = root["timeout"].asInt();
        return true;
    }
    return false;
}

// 需要根据最小连接数、最大连接数来不停的创建连接
// 若连接够用，则阻塞该线程
void ConnectionPool::produceConnection()
{
    while(true)
    {
        // RALL手法封装的互斥锁，初始化即加锁，析构即解锁
        // 模板类，指定模板参数为mutex，locker对象对互斥锁对象进行包装以自动完成加锁和解锁操作。
        // 当locker对象被创建后，自动的对互斥锁m_mutexQ加锁，locker析构时自动解锁。
        // 而locker对象定义在最外层的while循环中，因此出了这个while循环就会被析构
        unique_lock<mutex> locker(m_mutexQ);
        // 大于等于最小数量，说明不需要再生产了
        // 阻塞条件写在上面，或者阻塞条件作为一个回调函数指定给wait()方法，这里我们使用第一种
        // 这里为什么用while：假设连接池中还有一个连接就达到了最小连接数量，而当前有两个子线程阻塞在wait()函数处，若使用if那么两个子线程同时生产两个连接，就超过了最小连接数量。
        while((int)m_connectionQ.size() >= m_minSize) // .size()返回unsigned int，编译器会警告unsigned int与int进行比较的问题
        {
            m_cond.wait(locker); // 根据条件变量来阻塞线程（使用wait、wait_for或wait_until）
            // 什么时候唤醒阻塞的生产者线程呢？当消费者消费了一个连接时需要唤醒生产者
        }
        // 创建新的连接
        addConnection();
        m_cond.notify_all();
    }
}

// 周期性的回收空闲的数据库连接
void ConnectionPool::recycleConnection()
{
    while(true)
    {
        // this_thread命名空间，sleep_for为线程休眠函数，需要指定一个时间长度
        // duration类对应于时间长度，seconds是封装好的duration类型，单位为s
        // cchrono::seconds()对应的单位是秒，chrono::milliseconds(500)为毫秒, chrono为命名空间
        this_thread::sleep_for(chrono::milliseconds(500));
        lock_guard<mutex> locker(m_mutexQ);
        while((int)m_connectionQ.size() > m_minSize) // 当数据库连接数量大于最小连接数（只有满足了比最小连接数多，我们才会去判断是否有空闲连接），换句话讲，如果当前连接数量比最小连接数量还小，那么我们就需要创建新的连接，而非回收空闲连接了
    	{
        	// 先进先出，队头的连接肯定是最早创建的，空闲时间最久
            // 数据库中的连接包括创建后一直未被使用的，以及被线程使用完返还给连接池的（此时创建时间要重置）
            // 连接池连接的创建时间刷新函数应该放在哪里？应该是在MysqlConn.cpp中实现
        	MysqlConn* conn = m_connectionQ.front();
        	if(conn->getAliveTime() >= m_maxIdleTime)
        	{
            	// 如果超过空闲时间，就把这个连接删除掉
            	m_connectionQ.pop();
            	delete conn;
        	}
        	else
        	{
            	break;
        	}
    	}
    }
}

// 建立新连接时，需要将其被创建时的时间戳记录下来
// 在这里实现最大连接数量的控制？需要一个私有成员变量记录当前已经创建的连接。
void ConnectionPool::addConnection()
{
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_ip, m_user, m_passwd, m_dbName, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

// 若当前连接池中无数据库连接，那么就阻塞当前调用该函数的线程
// 通过条件变量调用wait()、wait_for()、wait_until()方法，后两种方法只会阻塞一段时间。
shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    unique_lock<mutex> locker(m_mutexQ);
    // 线程池队列为空，没有空闲连接了
    while(m_connectionQ.empty())
    {
        // wait_for阻塞一定的时间长度，一定时间后，对应线程自动解除阻塞。
        // wait方法会一直阻塞，直到手动调用解除阻塞的方法
        // m_cond为定义的条件变量
        // wait_for()函数接收两个参数：unique_lock<mutex> locker(m_mutexQ)包装之后的互斥锁、指定一个时间段
        // chrono命名空间中提供了一些通过duration封装后的类型，例如milliseconds、seconds等
        // 线程解除阻塞时有两种情况：一种是未达到阻塞时长，有了空闲连接，我们使用notify_one或notify_all唤醒了线程；另一种情况为达到阻塞时长，线程仍旧未被唤醒，返回一种叫timeout的状态。
        if(cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout)))
        {
            // 如果是第二种情况，表明当前连接池队列仍旧为空，此时可以返回一个空指针，或者让线程继续阻塞
            if(m_connectionQ.empty())
            {
                // return nullptr;
                continue; // continue会让线程重新回到阻塞状态
            }
        } 
        else
        {
            // 如果是第一种情况，则需要将该线程给唤醒，那么在哪里做呢？在生产者线程produceConnection()函数中做
        }
    }
           
    // 连接池队列不为空，那么就取出一个可用连接
    // 第一个参数为其管理的指针：MysqlConn* conn = m_connectionQ.front();
    // 第二个参数为匿名函数，定义共享指针对象中删除器的操作。[this]表示匿名函数内部捕捉外部变量的方式，用this表示可以使用ConnectionPool类内的所有成员，MysqlConn* conn为匿名函数的参数
    shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn){
        // 在对多线程共享的队列进行写操作和取操作时，都需要先进行加锁
        // 一种方法是在push操作上下，直接使用互斥锁mutex的lock()和unlock()方法
        // 另一种方法是跟之前一样，使用unique_lock模板类或者lock_guard模板类来对mutex进行封装，但是这种方法不能手动控制要锁的数据范围，因为他们是创建时加锁，析构时解锁。
        lock_guard<mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);                            
        });
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
    // 思考一个问题：当这个线程用完了数据库连接，如何再还给线程池呢
    // 方法一：通过一个额外的函数，接收一个数据库连接作为参数，将这个用完的连接插入到连接池队列中（refreshAliveTime和push方法）
    // 方法二：使用智能指针，无需额外函数，只需使用当前getConnection函数即可。具体做法是，将这个返回的conn指针通过智能指针（分为共享和独占两种）进行管理，这里我们用共享的指针，并通过匿名函数指定对象析构时，对所管理的conn指针的操作（归还连接池而不析构conmn指针）
}
           
ConnectionPool::~ConnectionPool()
{
   while(!m_connectionQ.empty())
   {
       MysqlConn* conn = m_connectionQ.front();
       m_connectionQ.pop();
       delete conn;
   }
}

// 带参数的构造函数，json文件中是数据库连接池的一些配置信息
ConnectionPool::ConnectionPool(string confFilePath)
{
    // 加载配置文件
    if(!parseJsonFile(confFilePath))
    {
        return;
    }
    // 首先初始化m_minSize个初始连接，并放入队列中
    for(int i = 0; i < m_minSize; i++)
    {
        addConnection();
    }
    // 线程相关，定义完两个线程后，需要给这两个线程指定任务函数（可以指定有名函数、匿名函数、类的静态成员函数、类的非静态成员函数、可调用对象例如仿函数）
    // 使用完需要将连接归还给线程池
    // 需要检测数据库连接池中的连接是否够用
    // 检测数据库连接池中的连接是否有较多空闲的连接
    // 这里我们指定子线程的任务函数为类的非静态成员函数，需要把任务函数的地址和所有者给出
    // 两个任务函数的所有者为当前的唯一实例对象，即this
    thread producer(&ConnectionPool::produceConnection, this);
    thread recycler(&ConnectionPool::recycleConnection, this);
    // 在当前程序的主线程中，还需要进行其他逻辑处理，因此不能调用这两个子线程的join()方法，会阻塞主线程
    // 为了不阻塞主线程，需要在线程创建时立即调用detach方法，将子线程与调用它的线程分离，彼此独立执行
    producer.detach();
    recycler.detach();
}