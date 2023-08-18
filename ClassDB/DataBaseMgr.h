#ifndef _DATABASEMGR_H_
#define _DATABASEMGR_H_

#include "ClassDataBase.h"
#include <array>

class DataBaseMgr
{
public:
	DataBaseMgr();

	~DataBaseMgr();

	void Start();

	void Close();

	std::array<ClassDataBase*, Config::pollingPthreadNum>* GetDBArryPtr();

	ClassDataBase* GetDBByIndex(int index);

	int GetDBCount();

private:

	//每条线程对应数组中的一个DB对象
	std::array<ClassDataBase*, Config::pollingPthreadNum> dbArry;
	std::array<ClassDataBase*, Config::pollingPthreadNum>* dbArryPtr;
	int count;											 //数量和轮询线程一致


/**********************************************************************************************************/


//此对象仅在启服和关服的时候连接并且加载以及存储数据
private:
	ClassDataBase* doLoadOffLineData;		//处理离线数据的数据库对象
	Redis* redisObj;						//处理离线数据的Redis缓存对象（离线数据放进Redis中）

public:
	
	//加载离线数据
	bool DoLoadOffLineData();

	//保存离线数据
	void SaveOffLineData();


/**********************************************************************************************************/

public:

	//保存所有用户数据(Client's Data redis->mysql)
	void SaveAllClientData();

	//保存所有公共模块的数据(Public Moudle's Data redis->mysql)
	void SaveAllPublicData();

private:
	ClassDataBase* doSaveData;
	Redis* redisSaveDataObj;

};

#endif