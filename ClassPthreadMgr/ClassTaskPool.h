#ifndef _CLASSTASKPOOL_H_
#define _CLASSTASKPOOL_H_

#include <map>
#include "../etc/Config.h"
#include <list>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <algorithm>

using namespace std;


//一个任务链表(一个链表对应一个工作线程)
class ClassTaskList
{
public:
	ClassTaskList();
	~ClassTaskList();

public:
	/*
	这里设置两个链表容器是为了一个用于接受socket来的信息, 一个给work线程执行, 等到work线程执行
	的那个容器空了之后,就互换,把空的用去接受socket信息,而刚刚装满信息的就用于给work线程去处理
	*/
	list<string> taskList_;
	list<string> taskList;
    pthread_mutex_t lock;            //线程锁（防止一边存放消息一边取消息造成bug）
    pthread_cond_t cond;             //条件变量（用来控制任务列表线程睡眠和唤醒）

	list<string>* pMessTaskList;	//指向接受socket信息的taskList
	list<string>* pWorkTaskList;	//指向work线程处理的taskList

public:
    int GetListSize();				 //获取用于接受socket信息的任务链表大小
	void SwapTaskList();			 //互换链表指针指向的容器
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