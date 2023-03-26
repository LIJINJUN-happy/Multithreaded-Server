#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

//Lua_API ��������Ҫ�ļ���C����ʽ����
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include "../../GlobalVariable.h"

//LuaVM����
class LuaBaseVm
{
public:
	int luaVmType;			//luaVm���ͣ��û���������xxxx��
	lua_State* luaState;	//luaVm����ָ��
	std::string moudleName;	//ģ������(�û�Ϊuidת������,����ģ����Ϊģ����)
	
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