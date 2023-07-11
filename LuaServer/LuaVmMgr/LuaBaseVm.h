#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

extern std::map<std::string, int> GLOBAL_UID_SOCKET_MAP;
extern std::map<std::string, Redis*> GLOBAL_UID_REDISOBJECT_MAP;
extern std::list<std::string> TIMER_LIST;
extern pthread_mutex_t TIMER_LIST_LOCK;

//LuaVM基类
class LuaBaseVm
{
public:
	int luaVmType;			//luaVm类型（用户，公共，xxxx）
	lua_State* luaState;	//luaVm对象指针
	std::string moudleName;	//模块名称(用户为uid转化而来,公共模块则为模块名)
	long lastGcTime = 0;	//模块最后一次GC的时间戳
	
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
	void LoadScritpFunction(lua_State* L);//加载Lua调用C++的函数
	long CheckGcTimeArrive();
	void Gc();
};


namespace LuaScript
{
	//Lua Send Net Message
	int LuaSendMessage(lua_State* L);

	//add event in timerList
	int LuaAddEventIntoTimerList(lua_State* L);
}


#endif