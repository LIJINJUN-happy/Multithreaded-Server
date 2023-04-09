#ifndef _LUAPUBLICVM_H_
#define _LUAPUBLICVM_H_

#include "LuaBaseVm.h"

//公共VM类（继承baseVm）
class LuaPublicVm:public LuaBaseVm
{
private:
	pthread_mutex_t lock;	 //Vm锁（防止多线程同时操作）
	pthread_mutex_t* lockPtr;//锁地址

public:
	virtual bool Init(std::string path);
	pthread_mutex_t* GetPublickVmMutex();

public:
	LuaPublicVm(int, std::string);
	~LuaPublicVm();
};

#endif