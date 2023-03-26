#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

//Lua_API 的三个主要文件以C程序方式编译
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include "../../GlobalVariable.h"

//LuaVM基类
class LuaBaseVm
{
public:
	int luaVmType;			//luaVm类型（用户，公共，xxxx）
	lua_State* luaState;	//luaVm对象指针
	std::string moudleName;	//模块名称(用户为uid转化而来,公共模块则为模块名)
	
public:
	LuaBaseVm();
	LuaBaseVm(int, std::string);

public:
	virtual ~LuaBaseVm();
	virtual bool Init();
	lua_State* GetLuaStatePtr();
	int GetLuaVmType();
};

#endif