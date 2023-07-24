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


	ClassDataBase* doLoadLoginOffLineMsg;	//处理登录待处理数据的数据库对象（登录待处理数据放进C++运行程序中即可）

public:
	
	//加载离线数据
	bool DoLoadOffLineData();

	//保存离线数据
	void SaveOffLineData();

	//加载登录待处理数据的数据
	bool DoLoadLoginOffLineMsg();

	//保存登录待处理数据的数据
	void SaveLoginOffLineMsg();

};

#endif