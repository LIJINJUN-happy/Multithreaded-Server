#include "ClassLuaVmMgr.h"

using std::cout;
using std::endl;

LuaVmMgr::LuaVmMgr()
{
    this->luaVmMap.clear();
    this->luaVmMapPtr = &(this->luaVmMap);
    return;
}

LuaVmMgr::LuaVmMgr(Global::LuaMoudleFilesInfo* p)
{
    this->luaMoudleFilesInfoPtr = p;
}

LuaVmMgr::~LuaVmMgr()
{
}

LuaBaseVm* LuaVmMgr::GetLuaVmByIndex(std::string strIndex)
{
    std::map<std::string, LuaBaseVm*>::iterator ptr;   
    ptr = luaVmMap.find(strIndex);
    if (ptr != luaVmMap.end())
    {
        LuaBaseVm* p = this->luaVmMap[strIndex];
        return p;
    }
    return nullptr;
}

LuaBaseVm* LuaVmMgr::GetLuaVmByIndex(long long uid)
{
    std::string strIndex = std::to_string(uid);
    LuaBaseVm* p = this->GetLuaVmByIndex(strIndex);
    return p;
}

std::map<std::string, LuaBaseVm*>* LuaVmMgr::GetLuaVmMapPtr()
{
    return this->luaVmMapPtr;
}

bool LuaVmMgr::AddLuaBaseVm(std::string strIndex, LuaBaseVm* vmPtr)
{
    std::map<std::string, LuaBaseVm*>::iterator ptr;
    ptr = luaVmMap.find(strIndex);
    if (ptr == luaVmMap.end())
    {
        this->luaVmMap[strIndex] = vmPtr;
        return true;
    }
    cout << "Vm " << strIndex << " Add Fail Already Exsit!" << endl;
    return false;
}

bool LuaVmMgr::DeleteLuaBaseVm(std::string strIndex)
{
    std::map<std::string, LuaBaseVm*>::iterator ptr;
    ptr = luaVmMap.find(strIndex);
    if (ptr != luaVmMap.end())
    {
        //在LuaMap中删除前,先找到vm对象将其析构
        LuaBaseVm* vmPtr = ptr->second;
        this->luaVmMap.erase(strIndex);
        delete vmPtr;
        return true;
    }
    cout << "Vm " << strIndex << " Delete Fail, Not Exsit!" << endl;
    return false;
}

bool LuaVmMgr::InitPublicMoudle()
{
    return true;
}

Global::LuaMoudleFilesInfo* LuaVmMgr::GetLuaMoudleFilesInfoPtr()
{
    return this->luaMoudleFilesInfoPtr;
}

int LuaVmMgr::GetLuaVmTypeByIndex(std::string strIndex)
{
    LuaBaseVm* p = this->luaVmMap[strIndex];
    int type = p->GetLuaVmType();
    return type;
}

int LuaVmMgr::GetLuaVmTypeByIndex(long long uid)
{
    std::string strIndex = std::to_string(uid);
    int type = this->GetLuaVmTypeByIndex(strIndex);
    return type;
}
