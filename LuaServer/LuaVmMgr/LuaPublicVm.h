#ifndef _LUAPUBLICVM_H_
#define _LUAPUBLICVM_H_

#include "LuaBaseVm.h"

//����VM�ࣨ�̳�baseVm��
class LuaPublicVm:public LuaBaseVm
{
public:
	virtual bool Init(std::string path);
public:
	LuaPublicVm(int, std::string);
	~LuaPublicVm();
};

#endif