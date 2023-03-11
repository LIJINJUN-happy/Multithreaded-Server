#include "ClassMonitor.h"

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
    map<string, Client*> *pSockfdMap = this->tcpNetObj->GetSockfdMap();
    if (pSockfdMap->size() <= 0)
    {
        return;
    }
    for (auto mapIter : (*pSockfdMap))
    {
        if (mapIter.second->CheckoutIfOnLine() == false)
        {
            cout << "客户端 " << mapIter.first << " 心跳间隔过大，服务器主动与之断开连接" << endl;
            tcpNetObj->CloseClientByFd(mapIter.first);
            Client* pClient = mapIter.second->GetMyself();
            delete pClient;
            pSockfdMap->erase(mapIter.first);
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