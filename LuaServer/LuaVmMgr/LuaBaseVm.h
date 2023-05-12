#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

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
	virtual bool Init(std::string path) {};
	virtual bool Init() {};
	lua_State* GetLuaStatePtr();
	int GetLuaVmType();

public:
	void LoadScritpFunction(lua_State* L);//加载Lua调用C++的函数
};


namespace LuaScript
{
	//Lua Send Net Message
	int LuaSendMessage(lua_State* L);
}


#endif