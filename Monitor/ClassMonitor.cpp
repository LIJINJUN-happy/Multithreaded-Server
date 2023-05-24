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

void ClassMonitor::BeginCheck()
{
    while (true)
    {
        //检测心跳
        this->CheckoutClientIfOnline();
        usleep((Config::CheckoutIntervalTime)*1000000);
    }
}