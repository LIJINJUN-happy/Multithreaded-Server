#include "ClassDataBase.h"

using std::cout;
using std::string;
using std::endl;

//构造函数
ClassDataBase::ClassDataBase()
{
    resultRes = NULL;
    row = 0;
    field = NULL;

    //初始化数据库对象
    if (mysql_init(&(this->mysql)) == NULL)
    {
        cout << "数据库初始化失败    " << mysql_error(&mysql) << endl;
        return;
    }

    //接入数据库
    if (!mysql_real_connect(&(this->mysql), Config::host.c_str(), Config::user.c_str(),
                            Config::passwd.c_str(), Config::db.c_str(), Config::port, NULL, 0))
    {
        cout << "数据接入失败    " << mysql_error(&mysql) << endl;
        return;
    }
}

//析构函数
ClassDataBase::~ClassDataBase()
{
    mysql_close(&(this->mysql));
}

//执行操作
bool ClassDataBase::DoCommand(string command)
{
    int res = mysql_query(&(this->mysql), command.c_str());
    if (res != 0)
    {
        cout << "数据库操作失败" << mysql_error(&mysql) << endl;
        return false;
    }
    return true;
}