#include "ClassPthread.h"

//构造函数
ClassPthread::ClassPthread()
{
    //初始化变量
    if (Config::pollingPthreadNum < 1 )
	{	
		cout << "线程数量不可以小于1" << endl;
		return ;
	}

    this->pollingPthreadNum = Config::pollingPthreadNum;
    this->pPthread = new vector<pthread_t*>;
    this->pTaskList = new vector<string>;
    
    //初始化锁
    pthread_mutex_init(&(this->lock),NULL);
}

//析构函数
ClassPthread::~ClassPthread()
{
    delete this->pPthread;
    delete this->pTaskList;
}

//获取线程数量
int ClassPthread::GetPollingPthreadNum()
{
    int num = this->pollingPthreadNum;
    return num;
}

//获取第几个线程地址
pthread_t* ClassPthread::GetOnePthreadByNum(int num)
{
    vector<pthread_t*> * p = this->pPthread;
    pthread_t* pid = ((*p)[num]);
    return pid;
}

//获取线程容器地址
vector<pthread_t*>* ClassPthread::GetPthreadVector()
{
    vector<pthread_t*> * p = this->pPthread;
    return p;
}

//获取任务列表容器地址
vector<string>* ClassPthread::GetTaskListVector()
{
    vector<string>* p = this->pTaskList;
    return p;
}

//添加线程地址入线程容器
void ClassPthread::AddPthread(pthread_t* tid)
{
    this->pPthread->push_back(tid);
}

//获取任务参数
Task ClassPthread::GetTaskArgs()
{
    Task task;
    task.pTaskList = this->pTaskList;
    task.lock = &(this->lock);
    return task;
}

void* CheckTaskList(void* args)
{
    vector<string>* pTaskList = ((Task*)args)->pTaskList;
    pthread_mutex_t* lock = ((Task*)args)->lock;
    pthread_t tid = pthread_self();
	while (1)
	{
        pthread_mutex_lock(lock);    //先上锁（防止其余线程同时争抢一个任务）
        //cout << "Pid :" << tid << "获取锁 " << "pTaskList=" << pTaskList << "  lock=" << lock << endl;
        if(pTaskList->size() >= 1)
        {
            string str = *(pTaskList->begin());         //取出任务容器首部的任务
            pTaskList->erase(pTaskList->begin());       //取出任务后删除（避免多次取出执行）
        }
        pthread_mutex_unlock(((Task*)args)->lock);  //解锁，释放资源让其余的线程获取任务
        //usleep(1000000);
    }
	return NULL;
}