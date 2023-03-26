#ifndef _CLASSLUAVMMGR_H_
#define _CLASSLUAVMMGR_H_

#include "LuaPublicVm.h"
#include "LuaPersonalVm.h"

//LuaVM������
class LuaVmMgr 
{
private:
	std::map<std::string, LuaBaseVm*> luaVmMap;
	std::map<std::string, LuaBaseVm*>* luaVmMapPtr;

public:
	LuaVmMgr();
	~LuaVmMgr();

public:
	LuaBaseVm* GetLuaVmByIndex(std::string strIndex);
	LuaBaseVm* GetLuaVmByIndex(long long uid);
	int GetLuaVmTypeByIndex(std::string strIndex);
	int GetLuaVmTypeByIndex(long long uid);
	std::map<std::string, LuaBaseVm*>* GetLuaVmMapPtr();
	bool AddLuaBaseVm(std::string strIndex, LuaBaseVm* vmPtr);
	bool DeleteLuaBaseVm(std::string strIndex);

public:
	void InitPublicMoudle();	//��ʼ��������ģ��
	
};

#endif