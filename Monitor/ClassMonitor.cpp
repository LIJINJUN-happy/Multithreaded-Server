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
    SafeMap<Client*>* pSockidMap = this->tcpNetObj->GetSockidMap();
    LuaVmMgr* luaVmptr =  this->pthreadObj->GetLuaVmMgrPtr();
    if (pSockfdMap->size() <= 0)
    {
        return;
    }
    auto mapIter = pSockfdMap->begin();
    for (; mapIter != pSockfdMap->end();)
    {
        if (mapIter->second->CheckoutIfOnLine() == false)
        {
            //移除fdMap内数据
            LOG.Log() << "客户端 " << mapIter->first << " 心跳间隔过大，服务器主动与之断开连接" << endl;
            Client* pClient = mapIter->second->GetMyself();
            std::string uid = pClient->GetClientUid();
            tcpNetObj->CloseClientByFd(mapIter->first);
            //LOG.Log() << "移除fdMap内数据后大小为：" << pSockfdMap->size() << std::endl;

            //移除idMap内数据
            if (pSockidMap->CheckoutIfExist(uid))
            {
                pSockidMap->erase(uid);
            }
            //LOG.Log() << "移除fdMap内数据后大小为：" << pSockidMap->size() << std::endl;

            //移除LuaVm
            luaVmptr->DeleteLuaBaseVm(uid);
            //LOG.Log() << "移除LuaVm内数据后大小为：" << luaVmptr->GetLuaVmMapPtr()->size() << std::endl;

            //移除UID-Socket的键值对容器内的数据
            extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;
            if (GLOBAL_UID_SOCKET_MAP.CheckoutIfExist(uid))
            {
                GLOBAL_UID_SOCKET_MAP.erase(uid);
            }
            //LOG.Log() << "移除GLOBAL_UID_SOCKET_MAP内数据后大小为：" << GLOBAL_UID_SOCKET_MAP.size() << std::endl;

            //移除Redis容器数据
            extern std::map<std::string, Redis*> GLOBAL_UID_REDISOBJECT_MAP;
            if (GLOBAL_UID_REDISOBJECT_MAP.find(uid) != GLOBAL_UID_REDISOBJECT_MAP.end())
            {
                Redis* redisPtr = GLOBAL_UID_REDISOBJECT_MAP.at(uid);
                GLOBAL_UID_REDISOBJECT_MAP.erase(uid);
                delete redisPtr;
                redisPtr = nullptr;
            }
            //LOG.Log() << "移除GLOBAL_UID_REDISOBJECT_MAP内数据后大小为：" << GLOBAL_UID_REDISOBJECT_MAP.size() << std::endl;

            //析构client指针所指向的内存
            delete pClient;

            mapIter = pSockfdMap->begin();//删除元素有重新赋值迭代器（删除后迭代器失效）
        }
        else
        {
            mapIter++; //检测心跳还在的话检测下一个
        }
    }
}

int ClassMonitor::CheckoutClientAmount()
{
    extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;
    extern std::map<std::string, Redis*> GLOBAL_UID_REDISOBJECT_MAP;
    extern ClassServer* SERVER_OBJECT;
    int num = SERVER_OBJECT->GetActorAmount();
    LOG.Log() << std::endl;
    LOG.Log() << "Online Actor Amount Is ：" << num << std::endl;
    LOG.Log() << "FdMap Amount Is ：" << this->tcpNetObj->GetSockfdMap()->size() << std::endl;
    LOG.Log() << "IdMap Amount Is ：" << this->tcpNetObj->GetSockidMap()->size() << std::endl;
    LOG.Log() << "LuaVm Amount Is ：" << this->pthreadObj->GetLuaVmMgrPtr()->GetPersonalVmAmount() << std::endl;
    LOG.Log() << "GLOBAL_UID_SOCKET_MAP Amount Is ：" << GLOBAL_UID_SOCKET_MAP.size() << std::endl;
    LOG.Log() << "GLOBAL_UID_REDISOBJECT_MAP Amount Is ：" << GLOBAL_UID_REDISOBJECT_MAP.size() << std::endl;
    return num;
}

void ClassMonitor::CheckoutLuaVmWithActorMap()
{
    /*
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
            extern SafeMap<int> GLOBAL_UID_SOCKET_MAP;
            if (GLOBAL_UID_SOCKET_MAP.CheckoutIfExist(uid))
            {
                std::string fd = std::to_string(GLOBAL_UID_SOCKET_MAP[uid]);
                GLOBAL_UID_SOCKET_MAP.erase(uid);

                extern std::map<std::string, Redis*> GLOBAL_UID_REDISOBJECT_MAP;
                auto itRedis = GLOBAL_UID_REDISOBJECT_MAP.find(uid);
                if (itRedis != GLOBAL_UID_REDISOBJECT_MAP.end())
                {
                    Redis* redisPtr = GLOBAL_UID_REDISOBJECT_MAP.at(uid);
                    GLOBAL_UID_REDISOBJECT_MAP.erase(uid);
                    delete redisPtr;
                    redisPtr = nullptr;
                }

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
    }*/
}

void ClassMonitor::CheckoutServerCondition(int num, std::array<int,4>& alist, std::array<std::string,4>& slist)
{
    for (int index = 0; index < 4; index++)
    {
        if (num >= alist[index])
        {
            continue;
        }
        else
        {
            int condition = index + 1;
            extern ClassServer* SERVER_OBJECT;
            SERVER_OBJECT->SetServerConnectCondition(condition);
            LOG.Log() << "ServerCondition Is ：" << condition << std::endl;
            LOG.Log() << "服务器状况 ：" << slist[index] << std::endl << std::endl;
            return;
        }
    }
    return;
}

void ClassMonitor::BeginCheck()
{
    std::array<int,4> actorAmountList{200,500,800,Config::maxSocketfd};
    std::array<std::string, 4> serverConditionList{ "流畅","正常","火热","爆满" };
    int totalActor = 0;
    while (true)
    {
        //检测心跳
        //this->CheckoutClientIfOnline();

        //检测Lua虚拟机和客户Map中数量是否均存在（有可能出现lua虚拟机存在，actor不存在的情况）
        //this->CheckoutLuaVmWithActorMap();

        //检查客户端连接数量
        totalActor = this->CheckoutClientAmount();

        //服務器狀況
        this->CheckoutServerCondition(totalActor, actorAmountList, serverConditionList);

        usleep((Config::CheckoutIntervalTime)*1000000);
    }
}