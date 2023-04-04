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
    this->luaVmMap.clear();
    this->luaVmMapPtr = &(this->luaVmMap);
    this->luaMoudleFilesInfoPtr = p;
    return;
}

LuaVmMgr::~LuaVmMgr()
{
}

bool LuaVmMgr::CheckLuaVmIsExistByIndex(std::string strIndex)
{
    if (this->luaVmMap.find(strIndex) != this->luaVmMap.end())
    {
        return true;
    }
    return false;
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
        vmPtr = nullptr;
        return true;
    }
    cout << "Vm " << strIndex << " Delete Fail, Not Exsit!" << endl;
    return false;
}

bool LuaVmMgr::InitPublicMoudle()
{
    Global::LuaMoudleFilesInfo* p = this->GetLuaMoudleFilesInfoPtr();
    for (auto it=p->GetMoudleInfo()->begin();it!= p->GetMoudleInfo()->end();it++)
    {
        if (it->second.first == Global::PERSONAL)
            continue;
        else
        {
            LuaPublicVm* L = new LuaPublicVm(Global::PUBLIC,it->first);//基类指针指向派生类公共虚拟机对象（可多态调用）
            bool resInit = L->Init(it->second.second);               //初始化虚拟机(由于声明了virtual 这里调用LuaPublicVm的Init)
            if (resInit == true)
            {
                std::cout << "Public Moudle Init Success : " << it->second.second << std::endl;
                this->AddLuaBaseVm(it->first, (LuaBaseVm*)L);
                continue;
            }
            else
            {
                std::cout << "Public Moudle Init Fail : " << it->second.second << std::endl;
                delete L;
                return false;
            }
        }
    }
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
