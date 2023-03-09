#include "ClassPthreadMgr.h"

//构造函数
ClassPthreadMgr::ClassPthreadMgr()
{
    //初始化变量
    if (Config::pollingPthreadNum < 1)
    {
        cout << "线程数量不可以小于1" << endl;
        return;
    }

    this->pollingPthreadNum = Config::pollingPthreadNum;
    this->pTaskPool = new ClassTaskPool();
}

//析构函数
ClassPthreadMgr::~ClassPthreadMgr()
{
}

//获取线程数量
int ClassPthreadMgr::GetPollingPthreadNum()
{
    int num = this->pollingPthreadNum;
    return num;
}

//获取第几个线程地址
pthread_t *ClassPthreadMgr::GetOnePthreadByNum(int num)
{
    list<pthread_t *> *p = &(this->pthreadList);
    pthread_t *pid = NULL;
    if (p->size() < num)
    {
        cout << "Erro - 获取的线程数大于线程数！" << endl;
        return pid;
    }

    list<pthread_t *>::iterator i = p->begin(); //迭代器指针
    for (int total = 0; total != num - 1; total++)
    {
        i++;
    }
    pid = *i;
    return pid;
}

//获取线程容器地址
list<pthread_t *> *ClassPthreadMgr::GetPthreadList()
{
    list<pthread_t *> *p = &(this->pthreadList);
    return p;
}

//获取任务列表容器地址
ClassTaskPool* ClassPthreadMgr::GetTaskPool()
{
    return this->pTaskPool;
}

//添加线程地址入线程容器
void ClassPthreadMgr::AddPthread(pthread_t *tid)
{
    this->pthreadList.push_back(tid);
}

//获取任务参数
Task* ClassPthreadMgr::GetTaskArgs(int index)
{
    ClassTaskPool* taskPoolPtr = this->GetTaskPool();
    ClassTaskList* taskListPtr = taskPoolPtr->GetTaskListByID(index);

    //创建Task对象
    //Task *task = new Task();
    shared_ptr<Task> task(new Task());
    task->pTaskList = &(taskListPtr->pWorkTaskList);
    task->lock = &(taskListPtr->lock);
    task->cond = &(taskListPtr->cond);
    return task;
}

//把信息传进任务列表容器
void ClassPthreadMgr::AddMsgIntoTaskPool(string msg)
{
    map<int, ClassTaskList*> p = this->pTaskPool->GetClassTaskMap();
    // cout << "msg:" << msg << "已存放进入任务列表" << endl;
}

//检测任务列表循环（用锁来获取资源防止线程争抢）
void *CheckTaskList(void *args)
{
    list<string> *pTaskList = *(((Task *)args)->pTaskList);
    pthread_mutex_t *lock = ((Task *)args)->lock;
    pthread_cond_t *cond = ((Task *)args)->cond;
    pthread_t tid = pthread_self();
    string stringMsg = "";

    pthread_mutex_lock(lock); //上锁
    // cout << "Pid :" << tid << "获取锁 " << "pTaskList=" << pTaskList << "  lock=" << lock << endl;
    while (true)
    {
        stringMsg.clear();
        if (pTaskList->size() < 1)
        {
            // cout<<"先解锁并等待条件唤醒（等待争抢互斥锁并上锁）"<<endl;
            pthread_cond_wait(cond, lock);
        }
        if (pTaskList->size() >= 1)
        {
            stringMsg = *(pTaskList->begin());
            pTaskList->erase(pTaskList->begin()); //取出任务后删除（避免多次取出执行）

            // cout << "Pid :" << tid << "  取任务:" << stringMsg << endl;
            // cout << "取完后任务列表数量为" << pTaskList->size() << endl;
        }

        //执行任务
        if (stringMsg.size() >= 1)
        {
        }
    }
    //pthread_mutex_unlock(((Task*)args)->lock); //解锁,其实这一步解锁还是不解锁已经无所谓了,可以屏蔽掉
    return NULL;
}