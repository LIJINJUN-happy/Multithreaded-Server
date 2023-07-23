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
	std::array<ClassDataBase*, Config::pollingPthreadNum> dbArry;
	std::array<ClassDataBase*, Config::pollingPthreadNum>* dbArryPtr;
	int count;											 //数量和轮询线程一致
};

#endif