#include "LuaBaseVm.h"

using std::cout;
using std::endl;

LuaBaseVm::LuaBaseVm()
{
	this->luaVmType = 0;//默认为0
	this->luaState = luaL_newstate();
	this->moudleName = "";
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



/*----------------------------------------Lua Call C++ Script Function-----------------------------------------------*/

int LuaScript::LuaSendMessage(lua_State* L)
{
	int fd = luaL_checknumber(L, -1);
	std::string jsonStr = luaL_checkstring(L, -1);
	LOG.Log() << "fd : " << fd << std::endl;
	LOG.Log() << "jsonStr : " << jsonStr << std::endl;
	char buf[256] = { 0 };
	memcpy(buf, jsonStr.c_str(), jsonStr.size());
	int sendSize = send(fd, buf, strlen(buf), 0);
	lua_pushnumber(L, sendSize); //返回发送的字节
	return sendSize;
}
