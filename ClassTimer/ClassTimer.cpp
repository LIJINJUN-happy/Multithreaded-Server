#include "ClassTimer/ClassTimer.h"

ClassTimer::ClassTimer()
{
    this->intervalTime = 1; //默认为1
}

ClassTimer::~ClassTimer()
{
}

ClassTimer::ClassTimer(int time)
{
    this->intervalTime = time;
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
    return true;
}

void ClassTimer::CheckoutOnceEventList()
{
}

void ClassTimer::CheckoutLoopEventList()
{
}

//定时器循环阻塞执行
void *TimerLooping(void *args)
{
    for (i = 0; i < 100; i++)
    {
        struct timeval temp;

        temp.tv_sec = seconds;
        temp.tv_usec = mseconds;

        select(0, NULL, NULL, NULL, &temp);
    }
    return NULL;
}