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
	/*
	lua_pcall(L, 0, 0, 0)
	if ()
	{
		std::cout << "Public Moudle Init Failed :" << path << std::endl;
		return false;
	}*/
	return true;
}

LuaPublicVm::LuaPublicVm(int type, std::string mName) :LuaBaseVm(type, mName)
{
}

LuaPublicVm::~LuaPublicVm()
{
}
