#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;
extern SafeMap<Redis*> GLOBAL_UID_REDISOBJECT_MAP;
extern std::list<std::string> TIMER_LIST;
extern pthread_mutex_t TIMER_LIST_LOCK;

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

	//����Lua����C++�ĺ���(type����������ȥ����Lua���������,����ָ����Lua��C++�����ĺ���)
	void LoadScritpFunction(lua_State* L, int type);

	long CheckGcTimeArrive();

	void Gc();
};


namespace LuaScript
{
	//Lua Send Net Message
	int LuaSendMessage(lua_State* L);

	//add event in timerList
	int LuaAddEventIntoTimerList(lua_State* L);

	//get Redis data(����UID��Redis�л�ȡjson����)
	int LuaGetDataFromRedis(lua_State* L);

	//Lua����UID����json����ȥRedis��
	int LuaSetDataToRedis(lua_State* L);

	//Lua����Key��Redis�л�ȡ����ģ���json����
	int LuaGetPublicDataFromRedis(lua_State* L);

	//Lua����Key���ù���ģ���json����ȥRedis��
	int LuaSetPublicDataToRedis(lua_State* L);
}


#endif