#ifndef _LUAPUBLICVM_H_
#define _LUAPUBLICVM_H_

#include "LuaBaseVm.h"

//¹«¹²VMÀà£¨¼Ì³ÐbaseVm£©
class LuaPublicVm:public LuaBaseVm
{
public:
	virtual Init();
public:
	LuaPublicVm(int, std::string);
	~LuaPublicVm();
};

#endif