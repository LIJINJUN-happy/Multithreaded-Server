#include "LuaBaseVm.h"

using std::cout;
using std::endl;

LuaBaseVm::LuaBaseVm()
{
	this->luaVmType = 0;//默认为0
	this->luaState = luaL_newstate();
	this->moudleName = "";
	this->lastGcTime = Global::GetNowTime();
}

LuaBaseVm::LuaBaseVm(int type, std::string mName)
{
	if (type != Global::PERSONAL && type != Global::PUBLIC)
	{
		LOG.Log() << "Type 类型不正确" << endl;
		return;
	}
	this->luaVmType = type;
	this->moudleName = mName;
	this->luaState = luaL_newstate();
	this->lastGcTime = Global::GetNowTime();
}

LuaBaseVm::~LuaBaseVm()
{
	lua_close(this->luaState);
	this->luaState = nullptr;
}

/*bool LuaBaseVm::Init(std::string path) //纯虚函数不需要定义
{
	return true;
}*/

lua_State* LuaBaseVm::GetLuaStatePtr()
{
	return this->luaState;
}

int LuaBaseVm::GetLuaVmType()
{
	return this->luaVmType;
}

long LuaBaseVm::GetLuaVMLastGcTime()
{
	return this->lastGcTime;
}

void LuaBaseVm::SetLuaVMLastGcTime(long time)
{
	this->lastGcTime = time;
	return;
}

long LuaBaseVm::CheckGcTimeArrive()
{
	long nowTime = Global::GetNowTime();
	long lastTime = this->GetLuaVMLastGcTime();
	long needFlushTime = lastTime + Config::LuaVMGcIntervalTime;
	if (nowTime >= needFlushTime)
		return nowTime;
	return 0;
}

void LuaBaseVm::Gc()
{
	long nowTime = CheckGcTimeArrive();
	if (nowTime == 0)
		return;

	//int size = lua_gc(this->luaState, LUA_GCCOUNT, 0);
	//LOG.Log() << "before collect Gc Vm size = " << size << std::endl;
	int collectRes = lua_gc(this->luaState, LUA_GCCOLLECT, 0);
	//size = lua_gc(this->luaState, LUA_GCCOUNT, 0);
	//LOG.Log() << "after collect Gc Vm size = " << size << " collectRes = " << collectRes << std::endl;
	this->SetLuaVMLastGcTime(nowTime);
	return;
}

void LuaBaseVm::LoadScritpFunction(lua_State* L)
{
	lua_register(L, "LuaSendMessage", LuaScript::LuaSendMessage);//Lua发送协议@ args（socket,jsonStr）
}



/*----------------------------------------Lua Call C++ Script Function-----------------------------------------------*/

int LuaScript::LuaSendMessage(lua_State* L)
{
	int top = lua_gettop(L);
	//LOG.Log() << "top : " << top << std::endl;
	std::string uid = luaL_checkstring(L, 1);
	auto it = ::GLOBAL_UID_SOCKET_MAP.find(uid);
	std::string jsonStr = luaL_checkstring(L, 2);
	if (it == ::GLOBAL_UID_SOCKET_MAP.end())
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	char buf[256] = { 0 };
	memcpy(buf, jsonStr.c_str(), jsonStr.size());
	int fd = it->second;
	//LOG.Log() << "fd : " << fd << std::endl;
	//LOG.Log() << "jsonStr : " << jsonStr << std::endl;
	int sendSize = send(fd, buf, strlen(buf), 0);
	lua_pushnumber(L, sendSize); //Net Send Message Size
	return 1;
}

int LuaScript::LuaAddEventIntoTimerList(lua_State* L)
{
	int top = lua_gettop(L);
	//LOG.Log() << "top : " << top << std::endl;
	std::string msgType = luaL_checkstring(L, 1);	//消息类型（Actor or System）
	std::string called = luaL_checkstring(L, 2);	//调用的模块名
	std::string fun = luaL_checkstring(L, 3);		//调用的模块中的函数名
	std::string uid = luaL_checkstring(L, 4);		//消息事件类型为Actor则Uid为用户id  System则是模块名
	std::string eventType = luaL_checkstring(L, 5);	//消息事件类型(LoopEvent or OnceEvent)
	std::string paraTime1 = luaL_checkstring(L, 6);	//时间参数1
	std::string paraTime2 = luaL_checkstring(L, 7);	//时间参数2

	//判断用户在不在线
	if (msgType == "Actor")
	{
		auto it = ::GLOBAL_UID_SOCKET_MAP.find(uid);
		if (it == ::GLOBAL_UID_SOCKET_MAP.end())
		{
			lua_pushnumber(L, 0);
			return 1;
		}
	}
	std::string msgEvent(msgType + "|" + called + "|" + fun + "|" + uid + "|" + eventType + "|" + paraTime1 + "|" + paraTime2 + "|");
	pthread_mutex_lock(&(::TIMER_LIST_LOCK));
	::TIMER_LIST.push_back(msgEvent);
	pthread_mutex_unlock(&(::TIMER_LIST_LOCK));
	lua_pushnumber(L, 1);
	return 1;
}
