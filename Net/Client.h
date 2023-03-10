#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../etc/Config.h"
#include "../GlobalVariable.h"

using namespace std;

class Client
{
private:
    Client* pMyself;        //指向自己
    int fd;                 //套接字
    string uid;             //用户id
    string ipAddr;          //客户端ip地址
    long lastHeartBeatTime; //最后的心跳时间戳
    //int intervalTime;     //允许距离上次心跳最大时间

    //用于防止被不同的线程操作,消息派发的时候可以根据这个变量派发相应任务链表
    int workPthreadIndex;    //被操作的线程index,-1的时候就是没有被操作,大于等于0则是在被操作中
    
    /*由于NetSocket是基于Epoll的，而且是边沿触发，一次性必须全部读完，但是可能存在粘包的情况
    也就是说，epoll_wait处理的时候可能有一部分数据未能读取完，所以先把前段部分数据保存下来*/
    string messageResidue;   //socket缓存中未接收完的前段信息

public:
    Client();
    ~Client();
    Client(int clientFd, string clientUid, string clientIp);
    void UpdateHeartBeatTime();
    bool CheckoutIfOnLine();
    Client* GetMyself();
};

#endif