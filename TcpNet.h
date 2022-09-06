#ifndef _TCPNET_H_
#define _TCPNET_H_  

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <pthread.h>
#include <iostream>
#include "Config.h"

using namespace std;

class TcpNet
{
private:
    vector<int>* pSockfdList;                   //套接字的容器（存放套接字描述符）
    int port;                                   //监听端口
    string addr;                                //地址字符串
    int maxConnect;                             //最大连接数
    bool Init();                                //初始化（依次执行socket bind listen accept）

public:
    TcpNet();                                   //构造函数
    ~TcpNet();                                  //析构函数
    void Start();                               //开始执行初始化以及监听
    void Stop();                                //关闭套接字断开链接
};

#endif