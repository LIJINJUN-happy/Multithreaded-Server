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

	//ÿ���̶߳�Ӧ�����е�һ��DB����
	std::array<ClassDataBase*, Config::pollingPthreadNum> dbArry;
	std::array<ClassDataBase*, Config::pollingPthreadNum>* dbArryPtr;
	int count;											 //��������ѯ�߳�һ��


/**********************************************************************************************************/


//�˶�����������͹ط���ʱ�����Ӳ��Ҽ����Լ��洢����
private:
	ClassDataBase* doLoadOffLineData;		//�����������ݵ����ݿ����
	Redis* redisObj;						//�����������ݵ�Redis��������������ݷŽ�Redis�У�

public:
	
	//������������
	bool DoLoadOffLineData();

	//������������
	void SaveOffLineData();


/**********************************************************************************************************/

public:

	//���������û�����(Client's Data redis->mysql)
	void SaveAllClientData();

	//�������й���ģ�������(Public Moudle's Data redis->mysql)
	void SaveAllPublicData();

private:
	ClassDataBase* doSaveData;
	Redis* redisSaveDataObj;

};

#endif