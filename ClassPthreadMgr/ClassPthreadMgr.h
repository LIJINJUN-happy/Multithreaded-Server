#ifndef _CLASSPTHREADMGR_H_
#define _CLASSPTHREADMGR_H_

#include "../etc/Config.h"
#include <list>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

typedef struct Task
{
    list<string> *pTaskList; //任务列表地址
    pthread_mutex_t *lock;   //锁地址
    pthread_cond_t *cond;    //条件变量地址
} Task;

//线程执行的程序
void *CheckTaskList(void *);

class ClassPthreadMgr
{
private:
    int pollingPthreadNum;       //线程数量
    list<pthread_t *> pPthread;  //线程容器，生成的线程都在这里
    list<string> pTaskList;      //任务池用来取任务
    pthread_mutex_t lock;        //线程锁（防止相互争抢任务池资源，以至于一个任务多次重复做多次）
    pthread_cond_t cond;         //条件变量（用来控制任务列表线程睡眠和唤醒）

public:
    ClassPthreadMgr();                         //构造函数
    int GetPollingPthreadNum();             //获取线程数量
    pthread_t *GetOnePthreadByNum(int num); //获取第几个线程地址
    list<pthread_t *> *GetPthreadList();    //获取线程容器地址
    list<string> *GetTaskList();            //获取任务列表容器地址
    void AddPthread(pthread_t *);           //添加线程地址入线程容器
    Task GetTaskArgs();                     //获取任务参数
    void AddMsgIntoTaskList(string);        //把信息传进任务列表容器
    ~ClassPthreadMgr();                        //析构函数
};

#endif