#ifndef _CLASSLUAVMMGR_H_
#define _CLASSLUAVMMGR_H_

#include "LuaPublicVm.h"
#include "LuaPersonalVm.h"

//LuaVM管理类
class LuaVmMgr 
{
private:
	std::map<std::string, LuaBaseVm*> luaVmMap;
	std::map<std::string, LuaBaseVm*>* luaVmMapPtr;

	Global::LuaMoudleFilesInfo* luaMoudleFilesInfoPtr;

public:
	LuaVmMgr();
	LuaVmMgr(Global::LuaMoudleFilesInfo*);
	~LuaVmMgr();

public:
	bool CheckLuaVmIsExistByIndex(std::string strIndex);
	LuaBaseVm* GetLuaVmByIndex(std::string strIndex);
	LuaBaseVm* GetLuaVmByIndex(long long uid);
	int GetLuaVmTypeByIndex(std::string strIndex);
	int GetLuaVmTypeByIndex(long long uid);
	std::map<std::string, LuaBaseVm*>* GetLuaVmMapPtr();
	bool AddLuaBaseVm(std::string strIndex, LuaBaseVm* vmPtr);
	bool DeleteLuaBaseVm(std::string strIndex);

public:
	bool InitPublicMoudle();	//初始化公共类模块
	Global::LuaMoudleFilesInfo* GetLuaMoudleFilesInfoPtr();
};

#endif