#include "DataBaseMgr.h"

using std::cout;
using std::endl;
using std::string;
using std::array;

extern ClassServer* SERVER_OBJECT;
extern SafeMap<Redis*> GLOBAL_UID_REDISOBJECT_MAP;
extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;

DataBaseMgr::DataBaseMgr()
{
	this->count = Config::pollingPthreadNum;
	//this->dbArry.assign();
	this->dbArryPtr = &(this->dbArry);

	this->doLoadLoginOffLineMsg = nullptr;
	this->doLoadOffLineData = nullptr;

	this->doSaveData = nullptr;
	this->redisSaveDataObj = nullptr;
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
	LOG.Log() << std::endl;
	LOG.Log() << "\033[36mBegin To SaveOffLineData................\033[0m" << std::endl;
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
		//LOG.Log() << "OffLineData Len = " << len << std::endl;
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
					//LOG.Log() << "OffLineData Docommand Find  = " << saveOffLineString << std::endl;

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
						//LOG.Log() << "OffLineData Docommand Update Or Insert  = " << saveOffLineString << std::endl;

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

			LOG.Log() << "\033[36mSaveOffLineData Finished\033[0m" << std::endl << std::endl;
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

void DataBaseMgr::SaveAllClientData()
{
	LOG.Log() << std::endl;
	LOG.Log() << "\033[36mBegin To SaveAllClientData................\033[0m" << std::endl;

	doSaveData = new ClassDataBase();
	if (!doSaveData)
	{
		LOG.Error() << "SaveAllClientData Command Worng With doSaveData new fail" << std::endl;
		return;
	}

	std::string uid = "";
	std::string dbString = "";
	redisSaveDataObj = nullptr;
	Global::LuaMoudleFilesInfo* filesInfoPtr = SERVER_OBJECT->GetLuaMoudleFilesInfoPtr(); //根据文件加载情分类况加载DB数据

	std::map<std::string, Redis*>::iterator ptr = GLOBAL_UID_REDISOBJECT_MAP.begin();
	for (ptr; ptr != GLOBAL_UID_REDISOBJECT_MAP.end();)
	{
		uid = ptr->first;
		redisSaveDataObj = ptr->second;
		redisSaveDataObj = ::GLOBAL_UID_REDISOBJECT_MAP.at(uid);

		for (auto it = filesInfoPtr->GetMoudleInfo()->begin(); it != filesInfoPtr->GetMoudleInfo()->end(); it++)
		{
			if (it->second.first == Global::PERSONAL)
			{
				std::string moudle = it->first;
				for (int index = 0; index < moudle.size(); index++)
				{
					moudle[index] = tolower(moudle[index]);
				}

				//判断数据库是否存在数据（结果决定操作是update还是Insert）
				dbString = DBCommand::LoadLuaDataFromMysql;
				dbString.insert(dbString.find('.'), moudle);
				dbString.insert(dbString.find('.', dbString.find('.') + 1) + 1, moudle);
				dbString.insert(dbString.find_last_of('.'), moudle);
				dbString.insert(dbString.find_last_of('=') + 2, uid);
				bool doCommandResult = doSaveData->DoCommand(dbString);
				if (doCommandResult == false)
				{
					LOG.Error() << "DataBase Command Worng With SaveAllClientData :" << moudle << std::endl;
					continue;
				}
				else
				{
					std::string data = redisSaveDataObj->get(uid + "_" + moudle);
					if (data.size() == 0)
					{
						continue;
					}

					int row = doSaveData->GetResultRow();
					//LOG.Log() << "Mysql Load Json's row = " << row << std::endl;
					if (row == 1)   //找到了就是证明有数据（update）
					{
						dbString = DBCommand::SaveLuaDataWithUpdate;
						dbString.insert(dbString.find('.') + 1, moudle);
						dbString.insert(dbString.find('.', dbString.find('.') + 1), moudle);
						dbString.insert(dbString.find('=') + 2, data);
						dbString.insert(dbString.find_last_of('.'), moudle);
						dbString.insert(dbString.find_last_of('=') + 2, uid);
					}
					else if (row == 0)   //找不到就是证明暂无数据（insert）
					{
						dbString = DBCommand::SaveLuaDataWithInsert;
						dbString.insert(dbString.find('.') + 1, moudle);
						dbString.insert(dbString.find('\'') + 1, uid);
						dbString.insert(dbString.find_last_of('\''), data);
					}
					else
					{
						continue;
					}
				}

				// LOG.Log() << "Mysql Load Lua Json's dbString = " << dbString << std::endl;
				doCommandResult = doSaveData->DoCommand(dbString);
				if (!doCommandResult)
				{
					LOG.Error() << "SaveAllClientData Wrong With  :" << dbString << std::endl;
					continue;
				}
				else
				{
					//删除Redis中的相关缓存（节省空间）
					redisSaveDataObj->release(uid, moudle);
				}

			}
		}

		GLOBAL_UID_REDISOBJECT_MAP.erase(uid);
		delete redisSaveDataObj;
		redisSaveDataObj = nullptr;

		ptr++;
	}

	LOG.Log() << "\033[36mSaveAllClientData Finished\033[0m" << std::endl << std::endl;
	return ;
}

void DataBaseMgr::SaveAllPublicData()
{

}

