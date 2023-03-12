#include "Client.h"

//默认构造函数
Client::Client()
{
    this->fd = -1;
    this->uid = "";
    this->ipAddr = "";
    this->lastHeartBeatTime = Global::GetNowTime();
    this->pMyself = this;
    this->messageResidue = "";
    this->workPthreadIndex = -1;
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
    this->pMyself = this;
    this->messageResidue = "";
    this->workPthreadIndex = -1;//默认一开始没有线程执行该用户的请求
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
    int diff = abs(nowTime - this->lastHeartBeatTime);

    //假如间隔时间大于允许的最大心跳间隔时间
    if (diff > Config::HeartbeatIntervalTime)
    {
        cout << "距离上次的心跳间隔为：" << diff << "  ,大于规定的" << Config::HeartbeatIntervalTime << endl;
        return false;
    }

    return true;
}

void Client::UpdateMessageResidue(string newString)
{
    this->messageResidue = newString;
    return;
}

string Client::GetMessageResidue()
{
    return this->messageResidue;
}

void Client::UpdateWorkPthreadIndex(int newIndex)
{
    this->workPthreadIndex = newIndex;
}

int Client::GetWorkPthreadIndex()
{
    return this->workPthreadIndex;
}

Client* Client::GetMyself()
{
    return this->pMyself;
}
