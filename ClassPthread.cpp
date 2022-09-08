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
    this->pPthread = new list<pthread_t*>;
    this->pTaskList = new list<string>;
    
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
    list<pthread_t*> * p = this->pPthread;
    pthread_t* pid = NULL;
    if (p->size() < num)
    {
        cout << "Erro - 获取的线程数大于线程数！" << endl;
        return pid;
    }
    
    list<pthread_t*>::iterator i = p->begin();//迭代器指针
    for (int total = 0; total != num - 1; total++)
    {
        i++;
    }
    pid = *i;
    return pid;
}

//获取线程容器地址
list<pthread_t*>* ClassPthread::GetPthreadList()
{
    list<pthread_t*> * p = this->pPthread;
    return p;
}

//获取任务列表容器地址
list<string>* ClassPthread::GetTaskList()
{
    list<string>* p = this->pTaskList;
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

//把信息传进任务列表容器
void ClassPthread::AddMsgIntoTaskList(string msg)
{
    this->pTaskList->push_back(msg);
    cout << "msg:" << msg << "已存放进入任务列表" << endl;
}

//检测任务列表循环（用锁来获取资源防止线程争抢）
void* CheckTaskList(void* args)
{
    list<string>* pTaskList = ((Task*)args)->pTaskList;
    pthread_mutex_t* lock = ((Task*)args)->lock;
    pthread_t tid = pthread_self();
	while (1)
	{
        pthread_mutex_lock(lock);                       //先上锁（防止其余线程同时争抢一个任务）
        //cout << "Pid :" << tid << "获取锁 " << "pTaskList=" << pTaskList << "  lock=" << lock << endl;
        if(pTaskList->size() >= 1)
        {
            string str = *(pTaskList->begin());         //取出任务容器首部的任务
            pTaskList->erase(pTaskList->begin());       //取出任务后删除（避免多次取出执行）
        }
        pthread_mutex_unlock(((Task*)args)->lock);      //解锁，释放资源让其余的线程获取任务
        //usleep(1000000);
    }
	return NULL;
}