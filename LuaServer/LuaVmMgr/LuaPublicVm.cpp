#include "LuaPublicVm.h"

using std::cout;
using std::endl;

bool LuaPublicVm::Init(std::string path)
{
	lua_State* L = this->GetLuaStatePtr();
	luaL_openlibs(L);

	/*载入定义在C++的函数,设置为Lua的全局函数*/
	this->LoadScritpFunction(L, Global::PUBLIC);

	int resLoad = luaL_loadfile(L, path.c_str());
	if (resLoad || lua_pcall(L, 0, 0, 0))
	{
		LOG.Log() << "Public Moudle Load Failed :" << path << std::endl;
		return false;
	}
	lua_getglobal(L, "DoInit_");
	lua_pushstring(L, Config::loadCodePathString.c_str());
	lua_pcall(L, 1, 1, 0);
	if (lua_isstring(L, -1))
	{
		LOG.Log() << "Public Moudle Init Failed :" << std::endl;
		LOG.Log() << lua_tostring(L,-1) << std::endl;
		return false;
	}
	else if (lua_isboolean(L, -1) && lua_toboolean(L, -1) == 1)
	{
		//LOG.Log() << "Lua Moudle DoInit_ Finish !" << std::endl;
		lua_settop(L, 0);//clear
	}
	else
	{
		LOG.Log() << "Init Waring With Other Erro :" << std::endl;
		return false;
	}
	return true;
}

pthread_mutex_t* LuaPublicVm::GetPublickVmMutex()
{
	return this->lockPtr;
}

LuaPublicVm::LuaPublicVm(int type, std::string mName) :LuaBaseVm(type, mName)
{
	pthread_mutex_init(&(this->lock), NULL);//初始化锁
	this->lockPtr = &(this->lock);
}

LuaPublicVm::~LuaPublicVm()
{
}
