#include "LuaPersonalVm.h"

using std::cout;
using std::endl;

bool LuaPersonalVm::Init()
{
	return true;
}

LuaPersonalVm::LuaPersonalVm(int type, std::string mName):LuaBaseVm(type, mName)
{
}

LuaPersonalVm::~LuaPersonalVm()
{
}
