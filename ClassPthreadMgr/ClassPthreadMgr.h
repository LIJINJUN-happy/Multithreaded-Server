#ifndef _CLASSPTHREADMGR_H_
#define _CLASSPTHREADMGR_H_

#include "ClassTaskPool.h"

typedef struct Task
{
    ClassTaskList* pTaskList;

    std::list<MsgPackage*>** pWorkList;   //任务列表指针的地址（因为假如单纯复制地址则是浅拷贝）
    std::list<MsgPackage*>** pMessList;   //接受socket信息列表指针的地址
    
    pthread_mutex_t *lock;   //锁地址
    pthread_cond_t *cond;    //条件变量地址

    //Lua管理器地址
    LuaVmMgr* luaVmMgrPtr;

    //db对象地址
    ClassDataBase* dbPtr;

} Task;

//线程执行的程序
void *CheckTaskList(void *);

class ClassPthreadMgr
{
private:
    int pollingPthreadNum;                //线程数量（同时也是任务池容器内任务链表的数量）
    std::list<pthread_t *> pthreadList;   //线程容器，生成的线程都在这里
    ClassTaskPool *pTaskPool;             //任务池
    LuaVmMgr* luaVmMgrPtr;                //Lua管理器地址

public:
    //构造函数
    ClassPthreadMgr(LuaVmMgr*);
    
    //析构函数
    ~ClassPthreadMgr();

    //线程容器操作
    int GetPollingPthreadNum();              //获取线程数量
    pthread_t *GetOnePthreadByNum(int num);  //获取第几个线程地址
    std::list<pthread_t *> *GetPthreadList();//获取线程容器地址
    void AddPthread(pthread_t*);             //添加线程地址入线程容器

    //任务容器操作
    ClassTaskPool* GetTaskPool();                           //获取任务列表容器地址
    Task* GetTaskArgs(int);                                 //获取任务池中某index索引任务链表参数
    void AddMsgIntoTaskPool(std::list<MsgPackage*>&,int);   //把信息传进任务列表容器
    int CheckMinTaskList();                                 //返回最少任务的任务链表索引

    //VM容器
    LuaVmMgr* GetLuaVmMgrPtr();
};

#endif