#ifndef _CLASSPTHREAD_H_
#define _CLASSPTHREAD_H_  

#include "Config.h"
#include <vector>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

typedef struct Task
    {
        vector<string>* pTaskList;
        pthread_mutex_t* lock;
    } Task;

//线程执行的程序
void* CheckTaskList(void *);

class ClassPthread
{
private:
    int pollingPthreadNum;          //线程数量
    vector<pthread_t*>* pPthread;   //线程容器，生成的线程都在这里
    vector<string>* pTaskList;      //任务池用来取任务
    pthread_mutex_t lock;           //线程锁（防止相互争抢任务池资源，以至于一个任务多次重复做多次）

public:
    ClassPthread();                         //构造函数
    int GetPollingPthreadNum();             //获取线程数量
    pthread_t* GetOnePthreadByNum(int num); //获取第几个线程地址
    vector<pthread_t*>* GetPthreadVector(); //获取线程容器地址
    vector<string>* GetTaskListVector();    //获取任务列表容器地址
    void AddPthread(pthread_t*);            //添加线程地址入线程容器
    Task GetTaskArgs();                     //获取任务参数
    ~ClassPthread();                        //析构函数
};

#endif