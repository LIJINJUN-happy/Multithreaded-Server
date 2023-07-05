#ifndef _CLASSTIMER_H_
#define _CLASSTIMER_H_

//#include "../ClassPthreadMgr/ClassPthreadMgr.h"
#include "../Net/ClassTcpNet.h"

extern std::list<std::string> TIMER_LIST;
extern pthread_mutex_t TIMER_LIST_LOCK;

//(循环事件子元素)
typedef struct LoopEvent
{
    int nowTime;        //当前累计second
    int tarTime;        //目标second
    MsgPackage* msgPack;//事件容器
} LoopEvent;

//（单次事件子元素）
typedef struct OnceEvent
{
    int tarHour;         //触发事件的hour
    MsgPackage* msgPack; //事件容器
} OnceEvent;

//定时器循环阻塞执行
void *TimerLooping(void *);

//比较小时数值大小
bool CompareDiffEvent(OnceEvent, OnceEvent);

class ClassTimer
{
private:
    std::list<OnceEvent> onceEventList;  //单次事件容器
    std::list<LoopEvent> loopEventList;  //循环事件列表
    int intervalTime;                    //时间间隔
    ClassTcpNet* tcpObj;                 //TCP网络对象地址（指针）

public:
    ClassTimer();
    ~ClassTimer();
    ClassTimer(int, ClassTcpNet* tcpObj);
    int GetIntervalTime();
    bool AddOnceEvent(std::string);
    bool AddLoopEvent(std::string);
    void CheckoutOnceEventList();
    void CheckoutLoopEventList();
    std::list<OnceEvent> *GetOnceEventListPtr();
    std::list<LoopEvent> *GetLoopEventListPtr();
};

#endif

//通过使用select()，来设置定时器；原理利用select()方法的第5个参数，第一个参数设置为0，三个文件描述符集都设置为NULL，第5个参数为时间结构体
// select定时器是阻塞的,在等待时间到来之前什么都不做