#pragma once
/*
项目文件说明：建立数据库连接池，维护一个连接队列。
    -- 连接池只需要一个实例，因此设计为单例模式的类
    -- 连接队列的访问是线程安全的，因此访问前要加互斥锁。
    -- 动态的创建连接（生产者）、销毁空闲连接、回收连接、使用连接（消费者）。
*/
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable> // 条件变量，用于阻塞与唤醒线程
#include "databaseConnect.h"
#include <memory> // 共享指针的头文件
#include <chrono> // 时钟对应的头文件
using namespace std;

class ConnectionPool {
public:
    static ConnectionPool* getConnectionPool(string confFilePath); // 获取该连接池对象--单例模式
    shared_ptr<MysqlConn> getConnection(); // 使用shared_ptr模板类获取数据库连接池中的一个连接，用共享的智能指针方便数据库连接的回收。
    // MysqlConn* getConnection(); // 使用这种方式获取连接池的一个数据库连接需要一个额外的连接回收函数。

    ~ConnectionPool(); // 释放数据库连接池，以及连接池队列内的一个个连接

private:
    ConnectionPool(string confFilePath); // 为了保证对象只被创建一次，需要将构造函数设置为私有，让它不被外部所访问，因此只能通过公有的静态函数getConnectionPool()来创建一个对象
    ConnectionPool(const ConnectionPool& obj) = delete; // 删除拷贝构造函数
    ConnectionPool(const ConnectionPool&& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool& obj) = delete; // 删除拷贝赋值（=）运算符重载函数，=delete为C++ 11新增特性

    bool parseJsonFile(string confFilePath); // 通过配置文件/bin/dbconf.json来自定义ip、user、passwd等信息。可以使用xml或json
    void produceConnection(); // 生产数据库连接（生产者）
    void recycleConnection(); // 销毁超时的数据库连接
    void addConnection(); // 创建新的连接，用于produceConnection函数和构造函数中

    // 若不使用confFilePath，则只能将json文件存放在与main可执行文件同文件夹下
    // 以下信息存储在一个json文件中，confFilePath用来指定其路径，
    string m_ip;             // 数据库服务器ip地址
    string m_user;           // 数据库服务器用户名
    string m_dbName;         // 数据库服务器的数据库名
    string m_passwd;         // 数据库服务器密码
    unsigned short m_port;   // 数据库服务器绑定的端口
    int m_minSize;           // 连接池维护的最小连接数
    int m_maxSize;           // 连接池维护的最大连接数
    int m_maxIdleTime;       // 连接池中连接的最大空闲时长
    int m_timeout;           // 连接池获取连接的超时时长，即线程阻塞等待的时间长度

    queue<MysqlConn*> m_connectionQ; // 数据库连接存储在队列中
    mutex m_mutexQ; // 使用独占的互斥锁mutex（C++11提供四种互斥锁）来保护连接池队列
    condition_variable m_cond; // 使用条件变量来对生产者和消费者线程进行阻塞。C++11提供两种条件变量，这里我们使用condition_variable
};