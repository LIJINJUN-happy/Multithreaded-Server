#include "ClassPthread.h"

ClassPthread::ClassPthread()
{
    //初始化变量
    this->pollingPthreadNum = Config::pollingPthreadNum;
    pPthread = new vector<pthread_t*>;
    pTaskList = new vector<int>;

    //生成并，启动线程
    this->CreatePthreadByNum(Config::pollingPthreadNum);
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

vector<int>* ClassPthread::GetTaskListVector()
{
    vector<int>* p = this->pTaskList;
    return p;
}

void ClassPthread::CreatePthreadByNum(int num)
{
    for (int i = 0; i < num; i++)
	{
		pthread_t tid = 0;
		int resulCreatePthread = pthread_create(&tid, NULL, CheckTaskList, 0);
		if (resulCreatePthread == 0)
		{
			cout << "第" << i+1 << "个线程:" << tid << "启动成功!" << endl;
			this->pPthread->push_back(&tid);
		}
		else
		{
			cout << "第" << i+1 << "个线程启动失败!" << endl;
		}
	}
}

void* CheckTaskList(void* agrs)
{
	while (1)
	{

	}
	return NULL;
}