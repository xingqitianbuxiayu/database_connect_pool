#include "databaseConnect.h"

MysqlConn::MysqlConn() {
    // 初始化数据库连接环境, m_conn是类的私有成员变量
    init_conn = mysql_init(nullptr);
    // 设置数据库使用的编码为utf8，防止中文乱码
    mysql_set_character_set(init_conn, "utf8");
}

MysqlConn::~MysqlConn() {
    // 释放初始化的数据库连接环境
    if(init_conn != nullptr) {
        mysql_close(init_conn);
    }
    // 释放结果集
    freeResult();
}

void MysqlConn::freeResult() {
    if(sqlQuery_result != nullptr) {
        mysql_free_result(sqlQuery_result);
        sqlQuery_result = nullptr;
    }
}

// 在cpp文件中实现时，默认形参unsigned int port = 3306编译时会报错
bool MysqlConn::connect(string ip, string user, string password, string dbName, unsigned int port) {
    MYSQL* ptr = mysql_real_connect(init_conn, ip.c_str(), user.c_str(), password.c_str(), dbName.c_str(), port, nullptr, 0);
    if(ptr != nullptr) return true;
    return false;
}

bool MysqlConn::update(string sql) {
    if(mysql_query(init_conn, sql.c_str())) {
        return false;
    }
    return true;
}

bool MysqlConn::query(string sql)
{
    // 每次查询之前，释放sqlQuery_result指向的内存地址，并将sqlQuery_result定义为空指针
    freeResult();
    if (mysql_query(init_conn, sql.c_str()))
    {
        return false;
    }
    // sqlQuery_result是类的私有成员变量, 用于存储查询结果集
    sqlQuery_result = mysql_store_result(init_conn);
    return true;
}

bool MysqlConn::next()
{
    if (sqlQuery_result != nullptr)
    {
        // 从查询结果集中获取一行记录
        sqlQuery_row = mysql_fetch_row(sqlQuery_result);
        if(sqlQuery_row != nullptr) return true;
    }
    return false;
}

string MysqlConn::value(int index)
{
    int columnCount = mysql_num_fields(sqlQuery_result);
    if(index >= columnCount || index < 0)
    {
        return string(); //超出列index范围，返回空字符串
    }
    char* val = sqlQuery_row[index];
    // 这种情况下，直接使用string(val)遇到\0就会终止，无法返回实际的长度
    // 因此需要调用mysql_fetch_length()函数，获取index对应列的字段值实际长度
    // 有了列值的实际长度，再使用string进行转换
    unsigned long length = mysql_fetch_lengths(sqlQuery_result)[index];
    return string(val, length);
}

bool MysqlConn::transaction()
{
    // false表示禁止mysql自动提交事务，我们可以手动提交事务
    return mysql_autocommit(init_conn, false);
}

bool MysqlConn::commit()
{
    return mysql_commit(init_conn);
}

bool MysqlConn::rollback()
{
    return mysql_rollback(init_conn);
}

void MysqlConn::refreshAliveTime()
{
    // conn_aliveTime为类的私有数据成员，使用系统的绝对时钟
    conn_aliveTime = steady_clock::now();
}

// 新的时间点减去创建时的时间点即为存活时长
long long MysqlConn::getAliveTime()
{
    // nanoseconds为封装duration类得到的位于chrono中的类型，单位级别为纳秒
    nanoseconds res = steady_clock::now() - conn_aliveTime;
    // 将纳秒转换成毫秒，需要用到duration_cast模板类
    milliseconds millsec = duration_cast<milliseconds>(res);
    // 需要的返回值类型为long long，因此将时间间隔转换成数量，单位为ms，即有多少个1ms
    return millsec.count();
}