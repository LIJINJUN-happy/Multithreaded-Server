#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "../ClassServer.h"

class ClassMonitor
{
private:
    ClassTcpNet *tcpNetObj;   // 自定义TCP类对象地址
    ClassPthreadMgr *pthreadObj; //自定义线程池类对象地址

public:
    ClassMonitor();
    ClassMonitor(ClassTcpNet *, ClassPthreadMgr *);
    ~ClassMonitor();
    void CheckoutClientIfOnline();          //检测客户端对象Client心跳间隔
    int  CheckoutClientAmount();            //检查客户端连接数量
    void CheckoutLuaVmWithActorMap();       //检测Lua虚拟机和客户Map是否均存在
    void BeginCheck();                      //检测触发函数
    void CheckoutServerCondition(int num, std::array<int,4>& list);  //檢測服務器人數情況（客戶量）
};

#endif