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
	LOG.Log() << "Begin To SaveOffLineData................" << std::endl;
	this->redisObj = new Redis();
	if (redisObj->connect(Config::RedisHost, Config::RedisPort) == false)
	{
		LOG.Log() << "Wrong With Redis SaveOffLineData" << std::endl;

		delete redisObj;
		redisObj = nullptr;

		return ;
	}

	this->doLoadOffLineData = new ClassDataBase();
	if (doLoadOffLineData)
	{
		const int len = redisObj->getKeysMatchingLen("*_OFFLINE_DATA*");
		LOG.Log() << "OffLineData Len = " << len << std::endl;
		if (len > 0)
		{
			std::vector<std::string> keysList{};
			std::string key = "";
			for (int i = 0; i < len; i++)
			{
				key = redisObj->getKeysMatchingResultWithIndex(i);
				keysList.push_back(key);
				key.clear();
			}
			redisObj->freeRedis();

			std::string saveOffLineString = "";
			std::string data = "";
			std::string uid = "";
			for (auto x: keysList)
			{
				saveOffLineString.clear();

				data.clear();
				data = redisObj->get(x);
				if (data.size() > 0)
				{
					uid.clear();
					uid = x.substr(0, x.size() - 13); //-13是因为"_OFFLINE_DATA"长度为13，只取uid

					saveOffLineString = DBCommand::checkOffLineWithSelect;
					saveOffLineString.replace(saveOffLineString.find('_'), 1, uid);
					LOG.Log() << "OffLineData Docommand Find  = " << saveOffLineString << std::endl;

					if (doLoadOffLineData->DoCommand(saveOffLineString))
					{
						int row = doLoadOffLineData->GetResultRow();

						//找到了就是证明有数据（update）
						if (row == 1)
						{
							saveOffLineString = DBCommand::saveOffLineWithUpdate;
							saveOffLineString.replace(saveOffLineString.find('_'), 1, data);
							saveOffLineString.replace(saveOffLineString.find_last_of('_'), 1, uid);

						}
						//找不到就是证明暂无数据（insert）
						else if (row == 0)
						{
							saveOffLineString = DBCommand::saveOffLineWithInsert;
							saveOffLineString.replace(saveOffLineString.find('_'), 1, uid);
							saveOffLineString.replace(saveOffLineString.find_last_of('_'), 1, data);
						}
						LOG.Log() << "OffLineData Docommand Update Or Insert  = " << saveOffLineString << std::endl;

						if (doLoadOffLineData->DoCommand(saveOffLineString))
						{
							redisObj->release(uid, "OFFLINE_DATA");
						}
						else
						{
							LOG.Error() << "SaveOffLineData Wrong With  :" << saveOffLineString << std::endl;
							continue;
						}

					}
					else
					{
						LOG.Error() << "DataBase Command Worng With SaveOffLineData UID :" << uid << std::endl;
						continue;
					}
				}
			}
		}

		delete (this->doLoadOffLineData);
		this->doLoadOffLineData = nullptr;

		delete redisObj;
		redisObj = nullptr;

		return ;
	}

	delete redisObj;
	redisObj = nullptr;

	return ;
}

bool DataBaseMgr::DoLoadLoginOffLineMsg()
{
	return false;
}

void DataBaseMgr::SaveLoginOffLineMsg()
{
}

