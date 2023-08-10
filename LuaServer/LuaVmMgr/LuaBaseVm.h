#ifndef _LUABASEVM_H_
#define _LUABASEVM_H_

#include "../../Net/ClassMsgPackage.h"

extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;
extern SafeMap<Redis*> GLOBAL_UID_REDISOBJECT_MAP;
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

	//加载Lua调用C++的函数(type的作用是用去区分Lua虚拟机类型,加载指定的Lua和C++交互的函数)
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

	//get Redis data(根据UID从Redis中获取json数据)
	int LuaGetDataFromRedis(lua_State* L);

	//Lua根据UID设置json数据去Redis中
	int LuaSetDataToRedis(lua_State* L);

	//Lua根据Key在Redis中获取公共模块的json数据
	int LuaGetPublicDataFromRedis(lua_State* L);

	//Lua根据Key设置公共模块的json数据去Redis中
	int LuaSetPublicDataToRedis(lua_State* L);
}


#endif