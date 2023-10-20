#pragma once
/*
- 项目文件说明：对mysql C API进行封装，实现数据库的增删改查操作。
    -- 初始化连接环境
    -- 连接mysql服务器
    -- 一系列增删改操作涉及到的事务处理
    -- 数据库查操作，涉及到对查询结果集的遍历
    -- 资源回收，对结果集和对初始的连接环境
*/
#include <iostream>
/*
- 如何才能识别到安装的mysql、jsoncpp等库
    -- 一种方式是在c_cpp_properties.json里面设置
    -- 一种是将文件夹拷到工程目录
*/
#include <mysql/mysql.h>
#include <chrono>

using namespace std; // 包含string、chrono（std::string、std::chrono）
using namespace chrono; // 时钟类中定义的命名空间

class MysqlConn {
public:
    // 初始化数据库连接，设置数据库编码
    MysqlConn();
    // 释放数据库连接、释放结果集（调用freeResult()）
    ~MysqlConn();
    // 连接数据库, 不用写MySQL* mysql，这个是已经写在私有数据中，因为好几个函数都需要访问它
    bool connect(string ip, string user, string password, string dbName, unsigned int port = 3306);
    // 数据库更新：insert、update、delete
    bool update(string sql);
    // 数据库查询：select。执行sql语句，获取查询结果集
    bool query(string sql);
    // 遍历查询结果集
    bool next();
    // 通过索引获取某条记录的列值
    string value(int index);
    // 事务处理：创建、回滚、提交
    bool transaction();
    bool commit();
    bool rollback();
    // 由于要回收连接，因此需要刷新起始的空闲时间点
    void refreshAliveTime();
    // 计算连接存活的总时长
    long long getAliveTime();
private:
    // 私有方法：释放数据查询结果集
    void freeResult();
    // 私有数据：初始的数据库连接、查询结果集、某条行记录、创建连接时的绝对时钟（时间戳）
    MYSQL* init_conn = nullptr; //MYSQL*为指向结构体MYSQL的指针
    MYSQL_RES* sqlQuery_result = nullptr; //MYSQL_RES*为指向结构体MYSQL_RES的指针
    MYSQL_ROW sqlQuery_row = nullptr; // MYSQL_ROW是typedef定义的char**指针，指向一个元素为char*的数组，char*指向一个字符数组，代表某列的列值
    steady_clock::time_point conn_aliveTime;
};
