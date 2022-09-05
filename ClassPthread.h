#include "Config.h"
#include <vector>
#include <pthread.h>

using namespace std;

class ClassPthread
{
private:
    int pollingPthreadNum;          //线程数量
    vector<pthread_t*>* pPthread;   //线程容器，生成的线程都在这里
    vector<int>* pTaskList;         //任务池用来取任务

public:
    ClassPthread();
    int GetPollingPthreadNum();
    pthread_t* GetOnePthreadByNum(int num);
    vector<pthread_t*>* GetPthreadVector();
    vector<int>* GetTaskListVector();
    ~ClassPthread();

protected:
    void CreatePthreadByNum(int);
    void* CheckTaskList(void *);
};


