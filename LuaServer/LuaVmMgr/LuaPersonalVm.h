#ifndef _LUAPERSONALVM_H_
#define _LUAPERSONALVM_H_

#include "LuaBaseVm.h"

//����VM�ࣨ�̳�baseVm��
class LuaPersonalVm :public LuaBaseVm
{
public:
	virtual bool Init(std::string path);
public:
	LuaPersonalVm(int, std::string);
	~LuaPersonalVm();
};

#endif