#ifndef _CLASSDATABASE_H_
#define _CLASSDATABASE_H_

#include "../Email/Mail.h"
#include <mysql/mysql.h>


class ClassDataBase
{
public:
    enum commanType
    {
        AddCommand = 1,     //增
        DeleteCommand = 2,  //删
        UpdateCommand = 3,  //改
        SelectCommand = 4   //查
    };

private:
    MYSQL mysql;          //数据库对象
    MYSQL_RES *resultRes; //获取最后一次查询语句字符串的SQL查询的结果集
    MYSQL_ROW row;        //从结果集中获取下一行
    MYSQL_FIELD *field;

public:
    ClassDataBase();
    ~ClassDataBase();

public:

    bool DoCommand(std::string);            //执行操作
    int GetResultRow();                     //返回结果行数（必须DoCommand返回true才可调用）
    int GetResultCount();                   //返回结果列数（必须DoCommand返回true才可调用）
    std::pair<int,int> GetResultRowCount(); //返回结果行列数量（必须DoCommand返回true才可调用）
    void release();
    MYSQL_ROW* GetNextRowInfo();

public:
    void PrintOutQuery();                   //打印数据出来（用于测试,但是需要在DoCommand返回true才可执行）
};

#endif

//分配或初始化MYSQL对象
// MYSQL *mysql_init(MYSQL *mysql)

//连接一个mysql服务器
/*MYSQL *mysql_real_connect (
MYSQL *mysql,               前面一个函数的返回的mysql实例句柄
const char*host,            要连接的数据库的主机，可以是ip地址或主机名
const char*user,            表示登录数据库的用户名
const char*passwd,          登录的密码
const char*db,              就是访问的数据库
unsigned intport,           mysql的tcp/ip端口默认是3306
const char*unix_socket,     表示连接类型
unsigned long client_flag)  暂时为0即可*/

//执行指定查询语句字符串的SQL查询
// int mysql_query(MYSQL *mysql,const char *query)

//获取最后一次查询语句字符串的SQL查询的结果集
// MYSQL_RES *mysql_store_result(MYSQL *mysql)

//返回结果集中的行数
// int mysql_num_rows(MYSQL_RES* result)

//返回结果集中的列数
// int mysql_field_count(MYSQL *mysql)

//返回结果集中的字段数
// int mysql_num_fields(MYSQL_RES* result)

//获取下一个列的类型
// MYSQL_FIELD* mysql_fetch_field(MYSQL_RES *result);

//从结果集中获取下一行
// MYSQL_ROW mysql_fetch_row(MYSQL_RES *result);

//取对应列的类型
// MYSQL_FIELD* mysql_fetch_field_direct(MYSQL_RES *result, int i);

//关闭MYSQL
// MYSQL *mysql_close(MYSQL *mysql)

//函数释放结果内存,该结果标识符是从 mysql_query() 返回的结果
// void mysql_free_result()

//unsigned int mysql_errno(MYSQL* mysql); 返回错误码
//const char* mysql_error(MYSQL* mysql); 返回错误信息描述