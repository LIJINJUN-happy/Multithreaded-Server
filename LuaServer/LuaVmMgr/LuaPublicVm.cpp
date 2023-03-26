#include "LuaPublicVm.h"

using std::cout;
using std::endl;

bool LuaPublicVm::Init()
{
	return true;
}

LuaPublicVm::LuaPublicVm(int type, std::string mName) :LuaBaseVm(type, mName)
{
}

LuaPublicVm::~LuaPublicVm()
{
}
