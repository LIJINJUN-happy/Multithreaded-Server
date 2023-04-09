#ifndef _LUAPUBLICVM_H_
#define _LUAPUBLICVM_H_

#include "LuaBaseVm.h"

//����VM�ࣨ�̳�baseVm��
class LuaPublicVm:public LuaBaseVm
{
private:
	pthread_mutex_t lock;	 //Vm������ֹ���߳�ͬʱ������
	pthread_mutex_t* lockPtr;//����ַ

public:
	virtual bool Init(std::string path);
	pthread_mutex_t* GetPublickVmMutex();

public:
	LuaPublicVm(int, std::string);
	~LuaPublicVm();
};

#endif