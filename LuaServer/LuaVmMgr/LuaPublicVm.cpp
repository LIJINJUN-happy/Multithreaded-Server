#include "LuaPublicVm.h"

using std::cout;
using std::endl;

bool LuaPublicVm::Init(std::string path)
{
	lua_State* L = this->GetLuaStatePtr();
	luaL_openlibs(L);
	int resLoad = luaL_loadfile(L, path.c_str());
	if (resLoad || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Public Moudle Load Failed :" << path << std::endl;
		return false;
	}
	lua_getglobal(L, "DoInit_");
	lua_pushstring(L, Config::LoadCodePathString.c_str());
	lua_pcall(L, 1, 1, 0);
	if (lua_isstring(L,-1))
	{
		std::cout << "Public Moudle Init Failed :" << std::endl;
		std::cout << lua_tostring(L,-1) << std::endl;
		return false;
	}
	else if(lua_isboolean(L,-1) && lua_toboolean(L, -1) == 1)
	{
		//std::cout << "Lua Moudle DoInit_ Finish !" << std::endl;
		lua_settop(L, 0);//clear
	}
	else
	{
		std::cout << "Init Waring With Other Erro :" << std::endl;
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
	pthread_mutex_init(&(this->lock), NULL);//³õÊ¼»¯Ëø
	this->lockPtr = &(this->lock);
}

LuaPublicVm::~LuaPublicVm()
{
}
