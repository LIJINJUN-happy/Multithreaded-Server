#ifndef _CLASSTASKPOOL_H_
#define _CLASSTASKPOOL_H_

#include <map>
#include "../etc/Config.h"
#include <list>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;


//一个任务链表(一个链表对应一个工作线程)
class ClassTaskList
{
public:
	ClassTaskList();
	~ClassTaskList();

public:
	list<string> taskList;           //任务池用来存任务
    pthread_mutex_t lock;            //线程锁（防止一边存放消息一边取消息造成bug）
    pthread_cond_t cond;             //条件变量（用来控制任务列表线程睡眠和唤醒）

public:
    int getListSize();				 //获取任务链表大小
};



//任务池（里面包含多个任务链表）
class ClassTaskPool
{
public:
	map<int,ClassTaskList*> * GetClassTaskMap(); //获取任务容器的地址
	ClassTaskList * GetTaskListByID(int index);  //根据index获取某任务链表类对象的地址

public:
	ClassTaskPool();
	~ClassTaskPool();

private:
	map<int,ClassTaskList*> taskPool;			//任务容器池
	int classTaskListNum;						//任务列表数量
};

#endif