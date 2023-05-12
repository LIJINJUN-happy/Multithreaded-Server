#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

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
	virtual bool Init(std::string path) {};
	virtual bool Init() {};
	lua_State* GetLuaStatePtr();
	int GetLuaVmType();

public:
	void LoadScritpFunction(lua_State* L);//����Lua����C++�ĺ���
};


namespace LuaScript
{
	//Lua Send Net Message
	int LuaSendMessage(lua_State* L);
}


#endif