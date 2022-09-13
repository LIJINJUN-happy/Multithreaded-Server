#include "ClassTimer/ClassTimer.h"

ClassTimer::ClassTimer()
{
    this->intervalTime = 1; //默认为1
    this->loopEventList = new list<LoopEvent>;
    this->onceEventList = new list<OnceEvent>;
}

ClassTimer::~ClassTimer()
{
}

ClassTimer::ClassTimer(int time)
{
    if (time <= 0)
    {
        cout << "间隔不可小于0秒";
        return;
    }
    this->intervalTime = time;
}

int ClassTimer::GetIntervalTime()
{
    return this->intervalTime;
}

bool ClassTimer::AddLoopEvent(int ntime, int ttime, string ev)
{
    if (ntime < 0 || ttime <= 0 || ev.size() < 1)
    {
        cout << "参数不正确" << endl;
        return false;
    }
    LoopEvent loopEvent;
    loopEvent.event = ev;
    loopEvent.nowTime = ntime;
    loopEvent.tarTime = ttime;
    this->loopEventList->push_back(loopEvent);
    return true;
}

bool ClassTimer::AddOnceEvent(int thour, string ev)
{
    if (thour < 0 || thour > 24 || ev.size() < 1)
    {
        cout << "参数不正确" << endl;
        return false;
    }
    OnceEvent onceEvent;
    onceEvent.tarHour = thour;
    onceEvent.event = ev;
    this->onceEventList->push_back(onceEvent);
    return true;
}

void ClassTimer::CheckoutOnceEventList()
{
}

void ClassTimer::CheckoutLoopEventList()
{
}

list<OnceEvent> *ClassTimer::GetOnceEventListPtr()
{
    return this->onceEventList;
}

list<LoopEvent> *ClassTimer::GetLoopEventListPtr()
{
    return this->loopEventList;
}

//定时器循环阻塞执行,select定时器是阻塞的,在等待时间到来之前什么都不做
void *TimerLooping(void *args)
{
    //间隔时间
    int seconds = ((ClassTimer *)args)->GetIntervalTime();
    struct timeval temp;
    temp.tv_sec = seconds;
    temp.tv_usec = 0;

    // list<OnceEvent> *onceList = ((ClassTimer *)args)->GetLoopEventListPtr(); //单次事件容器
    // list<LoopEvent> *loopList = ((ClassTimer *)args)->GetOnceEventListPtr(); //循环事件列表

    for (;;)
    {
        //检测单次容器
        ((ClassTimer *)args)->CheckoutOnceEventList();
        //检测循环事件
        ((ClassTimer *)args)->CheckoutLoopEventList();

        select(0, NULL, NULL, NULL, &temp);
    }
    return NULL;
}