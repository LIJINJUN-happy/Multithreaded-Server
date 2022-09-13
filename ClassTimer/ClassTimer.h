#ifndef _CLASSTIMER_H_
#define _CLASSTIMER_H_

#include <utility>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>

using namespace std;

//定时器循环阻塞执行
void *TimerLooping(void *);

//(循环事件子元素)
typedef struct LoopEvent
{
    int nowTime;  //当前累计second
    int tarTime;  //目标second
    string event; //事件容器
} LoopEvent;

//（单次事件子元素）
typedef struct OnceEvent
{
    int tarHour;  //触发事件的hour
    string event; //事件容器
} OnceEvent;

class ClassTimer
{
private:
    list<OnceEvent> *onceEventList; //单次事件容器
    list<LoopEvent> *loopEventList; //循环事件列表
    int intervalTime;               //时间间隔

public:
    ClassTimer();
    ~ClassTimer();
    ClassTimer(int);
    int GetIntervalTime();
    bool AddOnceEvent(int, string);
    bool AddLoopEvent(int, int, string);
    void CheckoutOnceEventList();
    void CheckoutLoopEventList();
    list<OnceEvent> *GetOnceEventListPtr();
    list<LoopEvent> *GetLoopEventListPtr();
};

#endif

//单次事件容器通过根据当前