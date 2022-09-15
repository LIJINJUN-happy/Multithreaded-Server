#include "Client.h"

//默认构造函数
Client::Client()
{
}

//析构函数
Client::~Client()
{
}

//自定义构造函数
Client::Client(int clientFd, string clientUid, string clientIp)
{
    this->fd = clientFd;
    this->uid = clientUid;
    this->ipAddr = clientIp;
    this->lastHeartBeatTime = Global::GetNowTime();
}

//更新心跳时间
void Client::UpdateHeartBeatTime()
{
    long nowTime = Global::GetNowTime();
    this->lastHeartBeatTime = nowTime;
}

//查看距离上次的心跳时间差是否超出范围
bool Client::CheckoutIfOnLine()
{
    long nowTime = Global::GetNowTime();
    int diff = nowTime - this->lastHeartBeatTime;

    //假如间隔时间大于允许的最大心跳间隔时间
    if (diff > Config::HeartbeatIntervalTime)
    {
        cout << "距离上次的心跳间隔为：" << diff << "  ,大于规定的" << Config::HeartbeatIntervalTime << endl;
        return false;
    }

    return true;
}