#include "ClassTaskPool.h"

ClassTaskList::ClassTaskList()
{
    //初始化锁和变量
    pthread_mutex_init(&(this->lock), NULL);
    pthread_cond_init(&(this->cond), NULL);
    taskList.clear();
}

ClassTaskList::~ClassTaskList()
{
}

int ClassTaskList::getListSize()
{
    int size = 0;
    size = this->taskList.size();
    return size;
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
