#include "ClassPthread.h"

ClassPthread::ClassPthread()
{
    //初始化变量
    this->pollingPthreadNum = Config::pollingPthreadNum;
    this->pPthread = new vector<pthread_t*>;
    this->pTaskList = new vector<string>;

}

ClassPthread::~ClassPthread()
{
    delete this->pPthread;
    delete this->pTaskList;
}

int ClassPthread::GetPollingPthreadNum()
{
    int num = this->pollingPthreadNum;
    return num;
}

pthread_t* ClassPthread::GetOnePthreadByNum(int num)
{
    vector<pthread_t*> * p = this->pPthread;
    pthread_t* pid = ((*p)[num]);
    return pid;
}

vector<pthread_t*>* ClassPthread::GetPthreadVector()
{
    vector<pthread_t*> * p = this->pPthread;
    return p;
}

vector<string>* ClassPthread::GetTaskListVector()
{
    vector<string>* p = this->pTaskList;
    return p;
}

bool ClassPthread::CreatePthreadByNum(int num)
{
    //初始化锁
    pthread_mutex_init(&(this->lock),NULL);
    Args args;
    args.pTaskList = this->pTaskList;
    args.lock = &(this->lock);
    
    for (int i = 0; i < num; i++)
	{
		pthread_t tid = 0;
		int resulCreatePthread = pthread_create(&tid, NULL, CheckTaskList, &args);
		if (resulCreatePthread == 0)
		{
			cout << "第" << i+1 << "个线程: =" << tid << "启动成功!" << endl;
			this->pPthread->push_back(&tid);
		}
		else
		{
			cout << "第" << i+1 << "个线程启动失败!" << endl;
			return false;
		}
	}
    return true;
}

bool ClassPthread::Start()
{
    int num = this->pollingPthreadNum;
    if (num < 1 )
	{	
		cout << "线程数量不可以小于1" << endl;
		return false;
	}  
    bool result = this->CreatePthreadByNum(num);
    if (result == false)
    {
        cout << "线程启动步骤失败" << endl;
    }
    else
    {
        cout << "线程启动步骤成功" << endl;
    }
    return result;
}

bool ClassPthread::Stop()
{

}

void* CheckTaskList(void* args)
{
    vector<string>* pTaskList = ((Args*)args)->pTaskList;
    pthread_mutex_t* lock = ((Args*)args)->lock;
	while (1)
	{
        pthread_mutex_lock(lock);    //先上锁（防止其余线程同时争抢一个任务）
        if(pTaskList->size() >= 1)
        {
            string str = *(pTaskList->begin());         //取出任务容器首部的任务
            pTaskList->erase(pTaskList->begin());       //取出任务后删除（避免多次取出执行）
        }
        pthread_mutex_unlock(((Args*)args)->lock);  //解锁，释放资源让其余的线程获取任务
	}
	return NULL;
}