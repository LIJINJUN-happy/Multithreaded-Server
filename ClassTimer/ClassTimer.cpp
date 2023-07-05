#include "ClassTimer.h"

using std::cout;
using std::string;
using std::list;
using std::endl;

ClassTimer::ClassTimer()
{
    this->intervalTime = 1; //默认为1
    this->tcpObj = NULL;
}

ClassTimer::~ClassTimer()
{
}

ClassTimer::ClassTimer(int time, ClassTcpNet* tcpObj)
{
    if (time <= 0)
    {
        LOG.Log() << "间隔不可小于1秒";
        return;
    }
    this->intervalTime = time;
    this->tcpObj = tcpObj;
}

int ClassTimer::GetIntervalTime()
{
    return this->intervalTime;
}

bool ClassTimer::AddLoopEvent(int ttime, string ev)
{
    if (ttime <= 0 || ev.size() < 1)
    {
        LOG.Log() << "参数不正确" << endl;
        return false;
    }
    LoopEvent loopEvent;
    loopEvent.event = ev;
    loopEvent.nowTime = 0;
    loopEvent.tarTime = ttime;
    this->loopEventList.push_back(loopEvent);
    return true;
}

bool ClassTimer::AddOnceEvent(int thour, string ev)
{
    if (thour < 0 || thour >= 24 || ev.size() < 1)
    {
        LOG.Log() << "参数不正确" << endl;
        return false;
    }
    OnceEvent onceEvent;
    onceEvent.tarHour = thour;
    onceEvent.event = ev;
    this->onceEventList.push_back(onceEvent);
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

    list<OnceEvent> *onceList = this->GetOnceEventListPtr();
    onceList->sort(CompareDiffEvent); //按tarhour来排序
    
    list<OnceEvent>::iterator index = (*onceList).begin();
    while (true)
    {
        if(index == onceList->end())
        {
            break;
        }

        if ((*index).tarHour == nowHour)
        {
            //this->pthreadObj->AddMsgIntoTaskPool((*index).event);
            onceList->erase(index);
            index = (*onceList).begin(); //删除后重新开始赋值遍历（指针会失效）
            continue;
        }

        index++;
    }
}

//检测循环事件
void ClassTimer::CheckoutLoopEventList()
{
    list<LoopEvent> *loopList = this->GetLoopEventListPtr();
    list<LoopEvent>::iterator index = (*loopList).begin();
    for (index; index != loopList->end(); index++)
    {
        int nowt = (*index).nowTime;
        if (nowt >= (*index).tarTime)
        {
            //this->pthreadObj->AddMsgIntoTaskPool((*index).event);
            (*index).nowTime = 0;//归零重新计算新的一轮循环
        }
        else
        {
            (*index).nowTime = nowt + 1;//下一秒
        }
        continue;
    }
}


list<OnceEvent> *ClassTimer::GetOnceEventListPtr()
{
    return &(this->onceEventList);
}

list<LoopEvent> *ClassTimer::GetLoopEventListPtr()
{
    return &(this->loopEventList);
}

//定时器循环阻塞执行
void *TimerLooping(void *args)
{
    //间隔时间
    int seconds = ((ClassTimer *)args)->GetIntervalTime();
    struct timeval temp;
    // list<LoopEvent> *loopList = ((ClassTimer *)args)->GetLoopEventListPtr();
    // list<OnceEvent> *onceList = ((ClassTimer *)args)->GetOnceEventListPtr();

    for (;;)
    {
        temp.tv_sec = seconds;
        temp.tv_usec = 0;
        select(0, NULL, NULL, NULL, &temp);

        if (::TIMER_LIST.size() >= 1)//检测全局定时器任务列表中是否有任务时间需要加载进入定时器
        {
            //上锁（防止取数据的时候有新数据的加入）

            //取数据
            auto it = ::TIMER_LIST.begin();
            while (it != ::TIMER_LIST.end())
            {

            }

            //解锁
        }

        //((ClassTimer *)args)->CheckoutOnceEventList();
        //((ClassTimer *)args)->CheckoutLoopEventList();
    }
    return NULL;
}

bool CompareDiffEvent(OnceEvent x, OnceEvent y)
{
    return x.tarHour < y.tarHour;
}