#ifndef _TCPNET_H_
#define _TCPNET_H_  

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <pthread.h>
#include <iostream>
#include "Config.h"

using namespace std;

void* epollListening(void *);                  //开始执行Epoll监听线程，把数据存进去Tasklist里面

class TcpNet
{
private:
    map<string,int>* pSockfdMap;                //套接字的容器（存放客户端套接字描述符）
    int serverSock;                             //服务端监听套接字描述符
    int port;                                   //监听端口
    string addr;                                //地址字符串
    int maxConnect;                             //最大连接队列数
    int maxSocketfd;                            //最大socket对象连接数
    int maxEpollEvent;                          //最大epoll监视事件返回个数

public:
    TcpNet();                                   //构造函数
    ~TcpNet();                                  //析构函数
    void Init();                                //初始化（依次执行socket bind listen accept）
};

#endif