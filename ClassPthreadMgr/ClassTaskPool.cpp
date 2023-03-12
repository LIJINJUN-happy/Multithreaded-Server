#include "ClassTaskPool.h"

ClassTaskList::ClassTaskList()
{
    //初始化锁和变量
    pthread_mutex_init(&(this->lock), NULL);
    pthread_cond_init(&(this->cond), NULL);

    taskList.clear();
    taskList_.clear();
    
    this->pMessTaskList = &(this->taskList_);
    this->pWorkTaskList = &(this->taskList);
}

ClassTaskList::~ClassTaskList()
{
}

int ClassTaskList::GetListSize()
{
    int messSize = this->pMessTaskList->size();
    int workSize = this->pWorkTaskList->size();
    return workSize + messSize;
}

void ClassTaskList::SwapTaskList()
{
    if (pWorkTaskList == &taskList)
    {
        pWorkTaskList = &taskList_;
        pMessTaskList = &taskList;
    }
    else if (pWorkTaskList == &taskList_)
    {
        pMessTaskList = &taskList_;
        pWorkTaskList = &taskList;
    }
}

map<int, ClassTaskList*>* ClassTaskPool::GetClassTaskMap()
{
    map<int, ClassTaskList*>* p = nullptr;
    p = &(this->taskPool);
    return p;
}

ClassTaskList* ClassTaskPool::GetTaskListByID(int index)
{
    ClassTaskList* p = nullptr;
    if (this->classTaskListNum >= index)
    {
        p = this->taskPool[index];
    }
    return p;
}

ClassTaskPool::ClassTaskPool()
{
    this->classTaskListNum = Config::pollingPthreadNum;
    for (int i = 0; i < this->classTaskListNum; i++)
    {
        this->taskPool[i] = new ClassTaskList();
    }
}

ClassTaskPool::~ClassTaskPool()
{
    for (int i = 0; i < this->classTaskListNum; i++)
    {
        delete this->taskPool[i];
    }
}
