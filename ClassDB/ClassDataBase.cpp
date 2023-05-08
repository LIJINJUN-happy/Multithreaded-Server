#include "ClassDataBase.h"

using std::cout;
using std::string;
using std::endl;

//构造函数
ClassDataBase::ClassDataBase()
{
    resultRes = NULL;
    //row = 0;
    field = NULL;

    //初始化数据库对象
    if (mysql_init(&(this->mysql)) == NULL)
    {
        LOG.Log() << "DataBase Init Fail !   " << mysql_error(&mysql) << endl;
        assert(false);
        return;
    }

    //接入数据库
    if (!mysql_real_connect(&(this->mysql), Config::host.c_str(), Config::user.c_str(),
                            Config::passwd.c_str(), Config::db.c_str(), Config::port, NULL, 0))
    {
        LOG.Log() << "DataBase Connect Fail !   " << mysql_error(&mysql) << endl;
        assert(false);
        return;
    }
    else
    {
        //LOG.Log() << "数据接入成功   " << Config::host.c_str() << "  " << Config::user.c_str() << "  " << Config::port << endl;
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
        LOG.Log() << "DataBase Command Fail ! " << mysql_error(&mysql) << endl;
        return false;
    }
    this->resultRes = mysql_store_result(&(this->mysql));
    return true;
}

int ClassDataBase::GetResultRow()
{
    return mysql_num_rows(this->resultRes);
}

int ClassDataBase::GetResultCount()
{
    return mysql_field_count(&(this->mysql));
}

std::pair<int, int> ClassDataBase::GetResultRowCount()
{
    int numRow = mysql_num_rows(this->resultRes);
    int numCount = mysql_field_count(&(this->mysql));
    return std::make_pair(numRow, numCount);
}

void ClassDataBase::release()
{
    mysql_free_result(this->resultRes);
    return;
}

MYSQL_ROW* ClassDataBase::GetNextRowInfo()
{
    this->row = mysql_fetch_row(this->resultRes);
    return this->row;
}

void ClassDataBase::PrintOutQuery()
{
    int resRow = this->GetResultRow();
    int count = this->GetResultCount();

    for (int i = 0; i < resRow; i++)
    {
        this->row = mysql_fetch_row(this->resultRes);
        for (int j = 0; j < count; j++)
        {
            printf("row[%d]=%s  ", i, row[j]);
        }
    }
    this->release();
}
