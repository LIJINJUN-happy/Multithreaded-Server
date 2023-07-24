#include "DataBaseMgr.h"

using std::cout;
using std::endl;
using std::string;
using std::array;

DataBaseMgr::DataBaseMgr()
{
	this->count = Config::pollingPthreadNum;
	//this->dbArry.assign();
	this->dbArryPtr = &(this->dbArry);

	this->doLoadLoginOffLineMsg = nullptr;
	this->doLoadOffLineData = nullptr;
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

std::array<ClassDataBase*, Config::pollingPthreadNum>* DataBaseMgr::GetDBArryPtr()
{
	return this->dbArryPtr;
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

bool DataBaseMgr::DoLoadOffLineData()
{
	this->redisObj = new Redis();
	if (redisObj->connect(Config::RedisHost, Config::RedisPort) == false)
	{
		LOG.Log() << "Wrong With Redis DoLoadOffLineData" << std::endl;
		
		delete redisObj;
		redisObj = nullptr;
		
		return false;
	}

	this->doLoadOffLineData = new ClassDataBase();
	if (doLoadOffLineData)
	{
		//执行数据库语句
		std::string dbString = DBCommand::loadOffLine;
		bool doCommandResult = doLoadOffLineData->DoCommand(dbString);
		if (!doCommandResult)
		{
			LOG.Log() << "Wrong With doCommandResult DoLoadOffLineData" << std::endl;
			goto FAIL_WITH_DOCOMMAND;
		}
		else
		{
			int row = doLoadOffLineData->GetResultRow();		//行
			int count = doLoadOffLineData->GetResultCount();	//列
			LOG.Log() << "DoLoadOffLineData row:"<< row << "  count:" << count << std::endl;
			if (row >= 1 && count == 2)
			{
				std::string uid = "", data = "";
				for (int i = 0; i < row; i++)
				{
					auto it = *(doLoadOffLineData->GetNextRowInfo());
					uid = it[0];
					data = it[1];
					LOG.Log() << "uid :" << uid << "  data:" << data << std::endl;

					//存进去Redis缓存中
					redisObj->set(uid + "_OFFLINE_DATA", data);
				}
			}
		}

		delete (this->doLoadOffLineData);
		this->doLoadOffLineData = nullptr;
		
		delete redisObj;
		redisObj = nullptr;

		return true;
	}

	FAIL_WITH_DOCOMMAND :
	delete redisObj;
	redisObj = nullptr;

	return false;
}

void DataBaseMgr::SaveOffLineData()
{
}

bool DataBaseMgr::DoLoadLoginOffLineMsg()
{
	return false;
}

void DataBaseMgr::SaveLoginOffLineMsg()
{
}

