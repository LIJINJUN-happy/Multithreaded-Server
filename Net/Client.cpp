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
    this->lastHeartBeatTime =
}

//更新心跳时间
void Client::UpdateHeartBeatTime()
{
}

//查看距离上次的心跳时间差是否超出范围
bool Client::CheckoutIfOnLine()
{
}