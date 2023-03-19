#include "ClassPthreadMgr.h"

using std::cout;
using std::string;
using std::list;
using std::endl;

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
    Task *task = new Task();
    task->pTaskList = taskListPtr;
    task->pMessList = &(taskListPtr->pMessTaskList);
    task->pWorkList = &(taskListPtr->pWorkTaskList);
    task->lock = &(taskListPtr->lock);
    task->cond = &(taskListPtr->cond);
    return task;
}

//把信息传进任务列表容器
void ClassPthreadMgr::AddMsgIntoTaskPool(list<MsgPackage*>& msgList,int minIndex)
{
    cout << "放入index为："<< minIndex << "的任务列表" << endl;
    ClassTaskList* pTaskList = this->pTaskPool->GetTaskListByID(minIndex);
    std::copy(msgList.begin(), msgList.end(), std::back_inserter(*(pTaskList->pMessTaskList)));

    //try_lock尝试判断pWorkTaskList是否已经空了
    int resTryLock = pthread_mutex_trylock(&(pTaskList->lock));
    if (resTryLock == 0)
    {
        pTaskList->SwapTaskList();
        pthread_mutex_unlock(&(pTaskList->lock)); //唤醒前先解锁，否则work线程被阻塞
        pthread_cond_signal(&(pTaskList->cond));
    }
    return;
}

int ClassPthreadMgr::CheckMinTaskList()
{
    //判断哪个任务列表最少任务并添加任务
    const int totalNum = Config::pollingPthreadNum;
    int minNum = this->pTaskPool->GetTaskListByID(0)->GetListSize();
    int minIndex = 0;
    for (int index = 0; index < totalNum; index++)
    {
        int listTaskNum = this->pTaskPool->GetTaskListByID(index)->GetListSize();
        if (minNum > listTaskNum)
        {
            minNum = listTaskNum;
            minIndex = index;
        }
    }
    return minIndex;
}

//检测任务列表循环（用锁来获取资源防止线程争抢）
void *CheckTaskList(void *args)
{
    ClassTaskList* pTaskList = ((Task*)args)->pTaskList;
    list<MsgPackage*>* pMessList = *(((Task*)args)->pMessList);
    list<MsgPackage*>* pWorkList = *(((Task*)args)->pWorkList);
    pthread_mutex_t *lock = ((Task *)args)->lock;
    pthread_cond_t *cond = ((Task *)args)->cond;
    pthread_t tid = pthread_self();
    string stringMsg = "";

    pthread_mutex_lock(lock); //上锁
    while (true)
    {
        stringMsg.clear();
        if (pWorkList->size() < 1)
        {
            if(pMessList->size() < 1)
            { 
                //cout << "work任务容器与mess容器均为空,先解锁休眠并等待任务到来时候被唤醒" << endl;
                pthread_cond_wait(cond, lock);
            }
            else if(pMessList->size() >= 1)
            {
                //cout << "work任务容器为空，但mess任务容器不为空,直接交换容器执行任务" << endl;
                pTaskList->SwapTaskList();
            }
            pMessList = *(((Task*)args)->pMessList);
            pWorkList = *(((Task*)args)->pWorkList);
        }

        if (pWorkList->size() >= 1)
        {
            MsgPackage* msgPtr = *(pWorkList->begin());
            stringMsg = msgPtr->GetCMD();
            pWorkList->erase(pWorkList->begin());
            delete msgPtr;
        }

        //执行任务
        if (stringMsg.size() >= 1)
        {
            //cout << "处理消息：" << stringMsg << endl;
            usleep(170000);
        }
    }
    //pthread_mutex_unlock(((Task*)args)->lock); //解锁,其实这一步解锁还是不解锁已经无所谓了,可以屏蔽掉
    return NULL;
}