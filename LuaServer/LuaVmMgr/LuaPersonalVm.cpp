#include "LuaPersonalVm.h"

using std::cout;
using std::endl;

bool LuaPersonalVm::Init(std::string path)
{
	return true;
}

LuaPersonalVm::LuaPersonalVm(int type, std::string mName):LuaBaseVm(type, mName)
{
}

LuaPersonalVm::~LuaPersonalVm()
{
}
