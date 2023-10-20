// main.cpp文件
#include <iostream>
#include <memory>
#include <thread>
#include "databaseConnect.h"
#include "connectionPool.h"
using namespace std;

// 使用非数据库连接池来创建连接
void op1(int begin, int end)
{
    for(int i = begin; i < end; i++)
    {
        MysqlConn conn;
        conn.connect("127.0.0.1", "root", "Root_123", "testdb");
        // 这里为了使主键的值随i依次变化，需要用到sprintf(char数组, 格式控制字符串, 变量)函数，可以将格式控制字符串赋值给char数组
        char sql[1024] = { 0 };
        sprintf(sql, "insert into person values(%d, 'Jerry', 'female', 25)", i);
        conn.update(sql); 
    }
}

// 使用数据库连接池来创建连接
void op2(ConnectionPool* pool, int begin, int end)
{
    for(int i = begin; i < end; i++)
    {
        shared_ptr<MysqlConn> conn = pool->getConnection();
        char sql[1024] = { 0 };
        sprintf(sql, "insert into person values(%d, 'Tom', 'male', 25)", i);
        conn->update(sql); 
    }
}

void test1(string confFilePath)
{ // 创建5000个数据库连接
// if 0 表示if下的代码暂时不起作用，运行test1会执行else下的代码，改为if 1即可执行
#if 0
    // 非连接池, 单线程
    steady_clock::time_point begin = steady_clock::now();
    op1(0, 5000);
    steady_clock::time_point end = steady_clock::now();
    // length是duration类型，length.count()可以返回这个时间区间有多少个纳秒。
    auto length = end - begin;
    cout << "非连接池, 单线程, 用时: " << length.count() << "纳秒, " << length.count() / 1000000 << "毫秒" << endl;
    
#else
    // 连接池,单线程
    ConnectionPool* pool = ConnectionPool::getConnectionPool(confFilePath);
    steady_clock::time_point begin = steady_clock::now();
    op2(pool, 0, 5000);
    steady_clock::time_point end = steady_clock::now();
    // length是duration类型，length.count()可以返回这个时间区间有多少个纳秒。
    auto length = end - begin;
    cout << "连接池, 单线程, 用时: " << length.count() << "纳秒, " << length.count() / 1000000 << "毫秒" << endl;
#endif
}

void test2(string confFilePath)
{
#if 1
    // 非连接池, 多线程
    // 在开启子线程之前，先进行一次数据库连接，防止多线程连接时出问题
    // 多个线程同时使用同一个用户名和密码登录时，MySQL会拒绝一些连接。
    MysqlConn conn;
    conn.connect("127.0.0.1", "root", "Root_123", "testdb");
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    // .join()以阻塞主线程
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    // length是duration类型，length.count()可以返回这个时间区间有多少个纳秒。
    auto length = end - begin;
    cout << "非连接池, 多线程, 用时: " << length.count() << "纳秒, " << length.count() / 1000000 << "毫秒" << endl;
    
#else
    // 连接池, 多线程
    ConnectionPool* pool = ConnectionPool::getConnectionPool(confFilePath);
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op2, pool, 0, 1000);
    thread t2(op2, pool, 1000, 2000);
    thread t3(op2, pool, 2000, 3000);
    thread t4(op2, pool, 3000, 4000);
    thread t5(op2, pool, 4000, 5000);
    // .join()以阻塞主线程
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    // length是duration类型，length.count()可以返回这个时间区间有多少个纳秒。
    auto length = end - begin;
    cout << "连接池, 多线程, 用时: " << length.count() << "纳秒, " << length.count() / 1000000 << "毫秒" << endl;
#endif
}

int query()
{
    MysqlConn conn;
    // conn.connect("localhost", "root", "Root_123", "testdb");
    if(!conn.connect("localhost", "root", "Root_123", "testdb")) return -1;
    string sql_ = "delete from person where id=3";
    bool flag_ = conn.update(sql_);
    cout << "flag_ value: " << flag_ << endl;
    
    string sql = "insert into person values(3, 'Tom', 'male', 25)";
    bool flag = conn.update(sql);
    cout << "flag value: " << flag << endl;
    
    sql = "select * from person";
    conn.query(sql);
    while(conn.next())
    {
        cout << conn.value(0) << ", " << conn.value(1) << ", " << conn.value(2) << ", " << conn.value(3) << endl;
    }
    return 0;
}

int main()
{
    string confFilePath = "/root/database_connect_pool/dbconf.json";
    // 单线程
    // test1(confFilePath);
    // 多线程
    test2(confFilePath);
    // query();
    return 0;
}