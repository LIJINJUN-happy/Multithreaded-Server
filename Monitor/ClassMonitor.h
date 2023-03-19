#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "../Net/ClassTcpNet.h"

class ClassMonitor
{
private:
    ClassTcpNet *tcpNetObj;   // 自定义TCP类对象地址
    ClassPthreadMgr *pthreadObj; //自定义线程池类对象地址

public:
    ClassMonitor();
    ClassMonitor(ClassTcpNet *, ClassPthreadMgr *);
    ~ClassMonitor();
    void CheckoutClientIfOnline(); //检测客户端对象Client心跳间隔
    void BeginCheck();             //检测触发函数
};

#endif