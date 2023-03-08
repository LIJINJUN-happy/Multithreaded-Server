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
    string uid;             //用户id
    string ipAddr;          //客户端ip地址
    long lastHeartBeatTime; //最后的心跳时间戳
    //int intervalTime;     //允许距离上次心跳最大时间

    //用于防止被不同的线程操作,消息派发的时候可以根据这个变量派发相应任务链表
    int workPthreadIndex;    //被操作的线程index,-1的时候就是没有被操作,大于等于0则是在被操作中

public:
    Client();
    ~Client();
    Client(int clientFd, string clientUid, string clientIp);
    void UpdateHeartBeatTime();
    bool CheckoutIfOnLine();
};

#endif