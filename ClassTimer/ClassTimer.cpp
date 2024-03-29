#include "ClassTimer.h"

using std::cout;
using std::string;
using std::list;
using std::endl;

ClassTimer::ClassTimer()
{
    this->intervalTime = 1; //默认为1
    this->tcpObj = NULL;
    this->taskPoolObj = nullptr;
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
    this->taskPoolObj = this->tcpObj->GetPthreadObj()->GetTaskPool();
}

int ClassTimer::GetIntervalTime()
{
    return this->intervalTime;
}

bool ClassTimer::AddLoopEvent(std::string ev)
{
    if (ev.size() < 1)
    {
        LOG.Log() << " ev 参数不正确" << endl;
        return false;
    }
    std::string msgType = ::Global::BreakDownByString(ev, "|");         //消息类型（Actor or System）
    std::string events = ::Global::BreakDownByString(ev, "|");		    //事件
    const std::string uid = ::Global::BreakDownByString(ev, "|");		//消息事件类型为Actor则Uid为用户id  System则是模块名
    std::string eventType = ::Global::BreakDownByString(ev, "|");	    //消息事件类型(LoopEvent or OnceEvent)
    std::string paraTime = ::Global::BreakDownByString(ev, "|");	    //时间参数
    if (eventType != "LoopEvent")
    {
        return false;
    }
    int ttime = atoi(paraTime.c_str());
    if (ttime <= 0)
    {
        LOG.Log() << " paraTime 参数不正确" << endl;
        return false;
    }

    void* oper = nullptr;
    if (msgType == "Actor")
    {
        try {
            oper = (void*)(this->tcpObj->GetSockidMap()->at(uid));
        }
        catch (const std::out_of_range& erro) {
            cout << endl << "When AddLoopEvent Out of Range Exception at " << erro.what();
            return false;
        }
    }
    else if(msgType == "System")
    {
        ;
    }
    else
    {
        LOG.Log() << "msgType 参数不正确" << endl;
        return false;
    }

    LoopEvent loopEvent;
    loopEvent.nowTime = 0;
    loopEvent.tarTime = ttime;
    loopEvent.msgPack = new MsgPackage(events, oper, (void*)(tcpObj->GetSockfdMap()), (void*)(tcpObj->GetSockidMap()), msgType.c_str());
    this->loopEventList.push_back(loopEvent);
    return true;
}

bool ClassTimer::AddOnceEvent(std::string ev)
{
    if (ev.size() < 1)
    {
        LOG.Log() << " ev 参数不正确" << endl;
        return false;
    }
    std::string msgType = ::Global::BreakDownByString(ev, "|");         //消息类型（Actor or System）
    std::string events = ::Global::BreakDownByString(ev, "|");		    //事件
    const std::string uid = ::Global::BreakDownByString(ev, "|");		//消息事件类型为Actor则Uid为用户id  System则是模块名
    std::string eventType = ::Global::BreakDownByString(ev, "|");	    //消息事件类型(LoopEvent or OnceEvent)
    std::string paraTime = ::Global::BreakDownByString(ev, "|");	    //时间参数
    if (eventType != "OnceEvent")
    {
        return false;
    }
    int thour = atoi(paraTime.c_str());
    if (thour < 0 || thour >= 24 )
    {
        LOG.Log() << " paraTime 参数不正确" << endl;
        return false;
    }

    void* oper = nullptr;
    if (msgType == "Actor")
    {
        try {
            oper = (void*)(this->tcpObj->GetSockidMap()->at(uid));
        }
        catch (const std::out_of_range& erro) {
            cout << endl << "When AddOnceEvent Out of Range Exception at " << erro.what();
            return false;
        }
    }
    else if (msgType == "System")
    {
        ;
    }
    else
    {
        LOG.Log() << "msgType 参数不正确" << endl;
        return false;
    }

    OnceEvent onceEvent;
    onceEvent.tarHour = thour;
    onceEvent.msgPack = new MsgPackage(events, oper, (void*)(tcpObj->GetSockfdMap()), (void*)(tcpObj->GetSockidMap()), msgType.c_str());
    this->onceEventList.push_back(onceEvent);
    //LOG.Log() << "After Add Into OnceEventList.size()  : " << this->onceEventList.size() << std::endl;
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
            this->AddMsgIntoTaskPool(index->msgPack);
            onceList->pop_front();
            index = (*onceList).begin(); //删除后重新开始赋值遍历（指针会失效）
            //LOG.Log() << "After Del From OnceEventList.size()  : " << this->onceEventList.size() << std::endl;
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
            MsgPackage* msg = new MsgPackage(index->msgPack->GetCMD(), index->msgPack->GetOperatePtr(), (void*)(tcpObj->GetSockfdMap()), (void*)(tcpObj->GetSockidMap()), index->msgPack->GetMsgType().c_str());
            this->AddMsgIntoTaskPool(msg);
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

void ClassTimer::AddMsgIntoTaskPool(MsgPackage* msgPack)
{
    extern ClassServer* SERVER_OBJECT;
    int taskListIndex = SERVER_OBJECT->GetMinTaskListIndex();
    if (msgPack->CheckMsgType("Actor") == true) //用户事件（需要增加任务数量 以及 选择当前的任务消息队列）
    {
        Client* pClient = ((Client*)(msgPack->GetOperatePtr()));
        int taskNum = pClient->GetClientTaskNum();
        if (taskNum <= 0)
        {   //证明该用户没有任务尚未完成（设置任务数最少的任务列表来存放）
            pClient->UpdateWorkPthreadIndex(taskListIndex);
        }
        else
        {
            //还有任务正在任务链表中未处理完
            taskListIndex = pClient->GetWorkPthreadIndex();
        }
        pClient->UpdateClientTaskNum(1);
    }
    else if(msgPack->CheckMsgType("System") == true) //系统模块事件（直接放入任务数量最少的列表）
    {
        ;
    }

    //传入任务列表容器
    ClassTaskList* pTaskList = taskPoolObj->GetTaskListByID(taskListIndex);
    pthread_mutex_t* putMessageLock = pTaskList->GetMessageLockPrt();
    pthread_mutex_lock(putMessageLock);//加锁,防止Epoll监听线程同时塞入数据（保证同一时间只有一个线程正在处理该任务列表容器）
    pTaskList->pMessTaskList->push_back(msgPack);

    //try_lock尝试判断pWorkTaskList是否已经空了
    int resTryLock = pthread_mutex_trylock(&(pTaskList->lock));
    if (resTryLock == 0)
    {
        pTaskList->SwapTaskList();
        pthread_mutex_unlock(&(pTaskList->lock)); //唤醒前先解锁，否则work线程被阻塞
        pthread_cond_signal(&(pTaskList->cond));
    }
    pthread_mutex_unlock(putMessageLock);
    return;
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

        //检测全局定时器任务列表中是否有任务时间需要加载进入定时器
        if (::TIMER_LIST.size() >= 1)
        {
            //上锁（防止取数据的时候有新数据的加入）
            pthread_mutex_lock(&(::TIMER_LIST_LOCK));

            //取数据
            auto it = ::TIMER_LIST.begin();
            while (it != ::TIMER_LIST.end())
            {
                if (it->find("LoopEvent") != std::string::npos )
                {
                    ((ClassTimer*)args)->AddLoopEvent(*it);
                }
                else if(it->find("OnceEvent") != std::string::npos)
                {
                    ((ClassTimer*)args)->AddOnceEvent(*it);
                }
                else
                {
                    LOG.Log() << "Event Not LoopEvent or OnceEvent" << endl;
                    break;
                }
                ::TIMER_LIST.pop_front();
                it = ::TIMER_LIST.begin();//重新赋值迭代器（删除后迭代器失效）
                LOG.Log() << "After Get Out , TIMER_LIST Size : " << ::TIMER_LIST.size() << "  TIMER_LIST_LOCK Address Is " << &(::TIMER_LIST_LOCK) << std::endl;
            }

            //解锁
            pthread_mutex_unlock(&(::TIMER_LIST_LOCK));
        }

        //检测一次性任务定时器列表
        ((ClassTimer *)args)->CheckoutOnceEventList();
        
        //检测循环任务定时器列表
        ((ClassTimer *)args)->CheckoutLoopEventList();
    }
    return NULL;
}

bool CompareDiffEvent(OnceEvent x, OnceEvent y)
{
    return x.tarHour < y.tarHour;
}