#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

extern std::map<std::string, int> GLOBAL_UID_SOCKET_MAP;

//LuaVM����
class LuaBaseVm
{
public:
	int luaVmType;			//luaVm���ͣ��û���������xxxx��
	lua_State* luaState;	//luaVm����ָ��
	std::string moudleName;	//ģ������(�û�Ϊuidת������,����ģ����Ϊģ����)
	long lastGcTime = 0;	//ģ�����һ��GC��ʱ���
	
public:
	LuaBaseVm();
	LuaBaseVm(int, std::string);

public:
	virtual ~LuaBaseVm();
	virtual bool Init(std::string path) {};
	virtual bool Init() {};
	lua_State* GetLuaStatePtr();
	int GetLuaVmType();
	long GetLuaVMLastGcTime();
	void SetLuaVMLastGcTime(long time);

public:
	void LoadScritpFunction(lua_State* L);//����Lua����C++�ĺ���
	long CheckGcTimeArrive();
	void Gc();
};


namespace LuaScript
{
	//Lua Send Net Message
	int LuaSendMessage(lua_State* L);
}


#endif