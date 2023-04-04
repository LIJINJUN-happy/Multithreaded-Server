#include "LuaPersonalVm.h"

using std::cout;
using std::endl;

bool LuaPersonalVm::Init(std::string path)
{
	cout << "into Init Fun" << endl;
	lua_State* L = this->GetLuaStatePtr();
	luaL_openlibs(L);
	int resLoad = luaL_loadfile(L, path.c_str());
	cout << "resLoad" << resLoad << endl;
	if (resLoad || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Personal Moudle Load Failed :" << path << std::endl;
		return false;
	}
	cout << "11111111111111111"<< endl;
	lua_getglobal(L, "DoInit_");
	lua_pushstring(L, Config::LoadCodePathString.c_str());
	lua_pcall(L, 1, 1, 0);
	if (lua_isstring(L, -1))
	{
		cout << "22222222222222" << endl;
		std::cout << "Personal Moudle Init Failed :" << std::endl;
		std::cout << lua_tostring(L, -1) << std::endl;
		return false;
	}
	else if (lua_isboolean(L, -1) && lua_toboolean(L, -1) == 1)
	{
		cout << "33333333333333333" << endl;
		std::cout << "Lua Moudle DoInit_ Finish !" << std::endl;
	}
	else
	{
		cout << "444444444444444" << endl;
		std::cout << "Init Waring With Other Erro :" << std::endl;
		return false;
	}
	cout << "5555555555555555" << endl;
	return true;
}

/*bool LuaPersonalVm::Init()
{
	return false;
}*/

LuaPersonalVm::LuaPersonalVm(int type, std::string mName):LuaBaseVm(type, mName)
{
}

LuaPersonalVm::~LuaPersonalVm()
{
}
