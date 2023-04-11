#include "DataBaseMgr.h"

using std::cout;
using std::endl;
using std::string;
using std::array;

DataBaseMgr::DataBaseMgr()
{
	this->count = Config::pollingPthreadNum;
	//this->dbArry.assign();
}

DataBaseMgr::~DataBaseMgr()
{
	this->Close();
}

void DataBaseMgr::Start()
{
	if (this->count < 1)
	{
		LOG.Log() << "Wrong With DB Start" << std::endl;
		return;
	}
	for (int i = 0; i < count; i++)
	{
		ClassDataBase* db = new ClassDataBase();
		this->dbArry.at(i) = db;
	}
	return;
}

void DataBaseMgr::Close()
{
	for (int i = 0; i < count; i++)
	{
		ClassDataBase* db = dbArry.at(i);
		if (db)
		{
			delete db;
			db = nullptr;
			this->dbArry.at(i) = nullptr;
		}
	}
	return;
}

ClassDataBase* DataBaseMgr::GetDBByIndex(int index)
{
	ClassDataBase* db = dbArry.at(index);
	if (db)
	{
		return db;
	}
	return nullptr;
}

int DataBaseMgr::GetDBCount()
{
	return this->count;
}

