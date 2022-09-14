#include "ClassTimer.h"

ClassTimer::ClassTimer()
{
    this->intervalTime = 1; //默认为1
    this->pthreadObj = NULL;
    this->loopEventList = new list<LoopEvent>();
    this->onceEventList = new list<OnceEvent>();
}

ClassTimer::~ClassTimer()
{
}

ClassTimer::ClassTimer(int time, ClassPthread *obj)
{
    if (time <= 0)
    {
        cout << "间隔不可小于0秒";
        return;
    }
    this->intervalTime = time;
    this->pthreadObj = obj;
    this->loopEventList = new list<LoopEvent>();
    this->onceEventList = new list<OnceEvent>();
}

int ClassTimer::GetIntervalTime()
{
    return this->intervalTime;
}

bool ClassTimer::AddLoopEvent(int ttime, string ev)
{
    if (ttime <= 0 || ev.size() < 1)
    {
        cout << "参数不正确" << endl;
        return false;
    }
    LoopEvent loopEvent;
    loopEvent.event = ev;
    loopEvent.nowTime = 0;
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

//检测单次容器
void ClassTimer::CheckoutOnceEventList()
{
    //获取时间
    time_t timep;
    time(&timep);
    struct tm *localTime = localtime(&timep);
    int nowHour = localTime->tm_hour;

    //按tarhour来排序
    onceList->sort(CompareDiffEvent);
    list<OnceEvent> *onceList = this->GetOnceEventListPtr();
    while (true)
    {
        list<OnceEvent> iterator index = (*onceList).begin();
        if ((*index).tarHour == nowHour)
        {
            this->pthreadObj->AddMsgIntoTaskList((*index).ev);
            onceList->pop_front();
            continue;
        }
        //由于排过序第一个不符合后面的都不符合
        else
        {
            break;
        }
    }
}

//检测循环事件
void ClassTimer::CheckoutLoopEventList()
{
    list<LoopEvent> *loopList = this->GetLoopEventListPtr();
    list<LoopEvent> iterator index = (*loopList).begin();
    for (index; index != loopList->end(); index++)
    {
        int nowt = (*index).nowTime;
        if (nowt >= (*index).tarTime)
        {
            this->pthreadObj->AddMsgIntoTaskList((*index).ev);
            (*index).nowTime = 0;
        }
        else
        {
            (*index).nowTime = nowt + 1;
        }
        continue;
    }
}

//

list<OnceEvent> *ClassTimer::GetOnceEventListPtr()
{
    return this->onceEventList;
}

list<LoopEvent> *ClassTimer::GetLoopEventListPtr()
{
    return this->loopEventList;
}

//定时器循环阻塞执行
void *TimerLooping(void *args)
{
    //间隔时间
    int seconds = ((ClassTimer *)args)->GetIntervalTime();
    struct timeval temp;
    temp.tv_sec = seconds;
    temp.tv_usec = 0;

    // list<LoopEvent> *loopList = ((ClassTimer *)args)->GetLoopEventListPtr();
    // list<OnceEvent> *onceList = ((ClassTimer *)args)->GetOnceEventListPtr();

    for (;;)
    {
        ((ClassTimer *)args)->CheckoutOnceEventList();
        ((ClassTimer *)args)->CheckoutLoopEventList();
        select(0, NULL, NULL, NULL, &temp);
    }
    return NULL;
}