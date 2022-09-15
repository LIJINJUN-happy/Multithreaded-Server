#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include "../etc/Config.h"
#include "../GlobalVariable.h"

using namespace std;

class Client
{
private:
    int fd;                 //套接字
    string uid;             // 用户id
    string ipAddr;          // 客户端ip地址
    long lastHeartBeatTime; //最后的心跳时间戳
    int intervalTime;       //允许距离上次心跳最大时间

public:
    Client();
    ~Client();
    Client(int clientFd, string clientUid, string clientIp);
    void UpdateHeartBeatTime();
    bool CheckoutIfOnLine();
};

#endif