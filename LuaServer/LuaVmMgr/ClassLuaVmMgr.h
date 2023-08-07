#ifndef _CLASSLUAVMMGR_H_
#define _CLASSLUAVMMGR_H_

#include "LuaPublicVm.h"
#include "LuaPersonalVm.h"

//LuaVM管理类
class LuaVmMgr 
{
private:
	SafeMap<LuaBaseVm*> luaVmMap;
	SafeMap<LuaBaseVm*>* luaVmMapPtr;

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

	SafeMap<LuaBaseVm*>* GetLuaVmMapPtr();

	bool AddLuaBaseVm(std::string strIndex, LuaBaseVm* vmPtr);

	bool DeleteLuaBaseVm(std::string strIndex);

public:

	//初始化公共类模块
	bool InitPublicMoudle();

	Global::LuaMoudleFilesInfo* GetLuaMoudleFilesInfoPtr();

	std::string GetPathByStringFromFilesInfo(std::string str);

	int GetPersonalVmAmount();
};

#endif