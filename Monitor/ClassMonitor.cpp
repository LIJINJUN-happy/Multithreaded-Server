#include "ClassMonitor.h"

ClassMonitor::ClassMonitor()
{
}

ClassMonitor::ClassMonitor(ClassTcpNet *tobj, ClassPthread *pobj)
{
    this->pthreadObj = pobj;
    this->tcpNetObj = tobj;
}

ClassMonitor::~ClassMonitor()
{
    delete this->pthreadObj;
    delete this->tcpNetObj;
}

//检测客户端对象Client心跳间隔
void ClassMonitor::CheckoutClientIfOnline()
{
    map<string, Client> *pSockfdMap = this->tcpNetObj->GetSockfdMap();
    if (pSockfdMap->size() <= 0)
    {
        return;
    }
    for (auto mapIter : (*pSockfdMap))
    {
        if (mapIter.second.CheckoutIfOnLine() == false)
        {
            tcpNetObj->CloseClientByFd(mapIter.first);
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