#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>

using namespace std;

class Client
{
private:
    int fd;                 //套接字
    string uid;             // udi
    string ipAddr;          // ip地址
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