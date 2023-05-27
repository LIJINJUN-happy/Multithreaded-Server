#include "ClassMonitor.h"

using std::cout;
using std::string;
using std::map;
using std::endl;

ClassMonitor::ClassMonitor()
{
}

ClassMonitor::ClassMonitor(ClassTcpNet *tobj, ClassPthreadMgr *pobj)
{
    this->pthreadObj = pobj;
    this->tcpNetObj = tobj;
}

ClassMonitor::~ClassMonitor()
{
}

//检测客户端对象Client心跳间隔
void ClassMonitor::CheckoutClientIfOnline()
{
    std::map<string, Client*> *pSockfdMap = this->tcpNetObj->GetSockfdMap();
    std::map<string, Client*>* pSockidMap = this->tcpNetObj->GetSockidMap();
    LuaVmMgr* luaVmptr =  this->pthreadObj->GetLuaVmMgrPtr();
    if (pSockfdMap->size() <= 0)
    {
        return;
    }
    for (auto mapIter : (*pSockfdMap))
    {
        if (mapIter.second->CheckoutIfOnLine() == false)
        {
            //移除fdMap内数据
            LOG.Log() << "客户端 " << mapIter.first << " 心跳间隔过大，服务器主动与之断开连接" << endl;
            Client* pClient = mapIter.second->GetMyself();
            std::string uid = pClient->GetClientUid();
            tcpNetObj->CloseClientByFd(mapIter.first);
            //LOG.Log() << "移除fdMap内数据后大小为：" << pSockfdMap->size() << std::endl;

            //移除idMap内数据
            if(pSockidMap->find(uid) != pSockidMap->end())
                pSockidMap->erase(uid);
            //LOG.Log() << "移除fdMap内数据后大小为：" << pSockidMap->size() << std::endl;

            //移除LuaVm
            luaVmptr->DeleteLuaBaseVm(uid);
            //LOG.Log() << "移除LuaVm内数据后大小为：" << luaVmptr->GetLuaVmMapPtr()->size() << std::endl;

            //移除UID-Socket的键值对容器内的数据
            extern std::map<std::string, int> GLOBAL_UID_SOCKET_MAP;
            GLOBAL_UID_SOCKET_MAP.erase(uid);

            //析构client指针所指向的内存
            delete pClient;
        }
        else
            continue;
    }
}

int ClassMonitor::CheckoutClientAmount()
{
    extern ClassServer* SERVER_OBJECT;
    int num = SERVER_OBJECT->GetActorAmount();
    LOG.Log() << "Online Actor Amount Is ：" << num << std::endl;
    return num;
}

void ClassMonitor::CheckoutLuaVmWithActorMap()
{
    int type = 0;
    std::string uid = "";
    LuaVmMgr* pLuaVmMap = this->pthreadObj->GetLuaVmMgrPtr();
    for (auto mapIter : (*(pLuaVmMap->GetLuaVmMapPtr())))
    {
        //共有LuaVm忽略
        type = mapIter.second->GetLuaVmType();
        if (type == Global::PUBLIC)
            continue;

        uid = mapIter.first;
        auto itIdMap = this->tcpNetObj->GetSockidMap()->find(uid);
        if (itIdMap == this->tcpNetObj->GetSockidMap()->end())
        {
            //移除luaVm
            this->pthreadObj->GetLuaVmMgrPtr()->DeleteLuaBaseVm(uid);

            //判断fdMap是否还有残留
            extern std::map<std::string, int> GLOBAL_UID_SOCKET_MAP;
            if (GLOBAL_UID_SOCKET_MAP.find(uid) != GLOBAL_UID_SOCKET_MAP.end())
            {
                std::string fd = std::to_string(GLOBAL_UID_SOCKET_MAP[uid]);
                GLOBAL_UID_SOCKET_MAP.erase(uid);

                auto itFdMap = this->tcpNetObj->GetSockfdMap()->find(fd);
                if (itFdMap != this->tcpNetObj->GetSockfdMap()->end())
                {
                    Client* pClient = itFdMap->second;
                    this->tcpNetObj->GetSockfdMap()->erase(fd);
                    if (pClient)
                        delete pClient;
                }
            }
        }
        uid.clear();
    }
}

void ClassMonitor::CheckoutServerCondition(int num, std::array<int,4>& list)
{
    for (int index = 0; index < 4; index++)
    {
        if (num >= list[index])
        {
            continue;
        }
        else
        {
            int condition = index + 1;
            extern ClassServer* SERVER_OBJECT;
            SERVER_OBJECT->SetServerConnectCondition(condition);
            LOG.Log() << "ServerCondition Is ：" << condition << std::endl;
            return;
        }
    }
    return;
}

void ClassMonitor::BeginCheck()
{
    std::array<int,4> actorAmountList{200,500,800,Config::maxSocketfd};
    int totalActor = 0;
    while (true)
    {
        //检测心跳
        this->CheckoutClientIfOnline();
        //检测Lua虚拟机和客户Map中数量是否均存在（有可能出现lua虚拟机存在，actor不存在的情况）
        this->CheckoutLuaVmWithActorMap();
        //检查客户端连接数量
        totalActor = this->CheckoutClientAmount();
        //服務器狀況
        this->CheckoutServerCondition(totalActor, actorAmountList);

        usleep((Config::CheckoutIntervalTime)*1000000);
    }
}