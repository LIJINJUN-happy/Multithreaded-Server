#ifndef _CLASSTASKPOOL_H_
#define _CLASSTASKPOOL_H_

#include "../Net/Gate.h"

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
	std::list<MsgPackage*> taskList_;
	std::list<MsgPackage*> taskList;
    pthread_mutex_t lock;            //线程锁（防止一边存放消息一边取消息造成bug）
    pthread_cond_t cond;             //条件变量（用来控制任务列表线程睡眠和唤醒）

	std::list<MsgPackage*>* pMessTaskList;	//指向接受socket信息的taskList
	std::list<MsgPackage*>* pWorkTaskList;	//指向work线程处理的taskList

public:

	//获取用于接受socket信息的任务链表大小
    int GetListSize();
	
	//互换链表指针指向的容器
	void SwapTaskList();

public:

	//信息添加锁（由于Timer定时器类和Socket都要往人物列表存放信息,避免冲突）
	pthread_mutex_t putMessageLock;

	pthread_mutex_t* GetMessageLockPrt();
};



//任务池（里面包含多个任务链表）
class ClassTaskPool
{
public:

	//获取任务容器的地址
	std::map<int,ClassTaskList*> * GetClassTaskMap();

	//根据index获取某任务链表类对象的地址
	ClassTaskList * GetTaskListByID(int index);

public:
	ClassTaskPool();
	~ClassTaskPool();

private:
	std::map<int,ClassTaskList*> taskPool;		//任务容器池
	int classTaskListNum;						//任务列表数量
};

#endif