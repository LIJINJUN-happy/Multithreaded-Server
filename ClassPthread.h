#include "Config.h"
#include <vector>
#include <pthread.h>
#include <iostream>

using namespace std;

//线程执行的程序
void* CheckTaskList(void *);

class ClassPthread
{
private:
    int pollingPthreadNum;          //线程数量
    vector<pthread_t*>* pPthread;   //线程容器，生成的线程都在这里
    vector<int>* pTaskList;         //任务池用来取任务

public:
    ClassPthread();                         //构造函数
    int GetPollingPthreadNum();             //获取线程数量
    pthread_t* GetOnePthreadByNum(int num); //获取第几个线程地址
    vector<pthread_t*>* GetPthreadVector(); //获取线程容器地址
    vector<int>* GetTaskListVector();       //获取任务列表容器地址
    ~ClassPthread();                        //析构函数
    bool Start();                           //开始执行线程
    bool Stop();                            //停止线程

protected:
    bool CreatePthreadByNum(int);           //创建线程
};


