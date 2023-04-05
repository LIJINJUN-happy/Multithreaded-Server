#include "ClassPthreadMgr.h"

using std::cout;
using std::string;
using std::list;
using std::endl;

//构造函数
ClassPthreadMgr::ClassPthreadMgr(LuaVmMgr* luaVmMgrP)
{
    //初始化变量
    if (Config::pollingPthreadNum < 1)
    {
        cout << "线程数量不可以小于1" << endl;
        return;
    }

    this->pollingPthreadNum = Config::pollingPthreadNum;
    this->pTaskPool = new ClassTaskPool();

    //赋值Lua模块管理系统指针
    this->luaVmMgrPtr = luaVmMgrP;
}

//析构函数
ClassPthreadMgr::~ClassPthreadMgr()
{
}

//获取线程数量
int ClassPthreadMgr::GetPollingPthreadNum()
{
    int num = this->pollingPthreadNum;
    return num;
}

//获取第几个线程地址
pthread_t *ClassPthreadMgr::GetOnePthreadByNum(int num)
{
    list<pthread_t *> *p = &(this->pthreadList);
    pthread_t *pid = NULL;
    if (p->size() < num)
    {
        cout << "Erro - 获取的线程数大于线程数！" << endl;
        return pid;
    }

    list<pthread_t *>::iterator i = p->begin(); //迭代器指针
    for (int total = 0; total != num - 1; total++)
    {
        i++;
    }
    pid = *i;
    return pid;
}

//获取线程容器地址
list<pthread_t *> *ClassPthreadMgr::GetPthreadList()
{
    list<pthread_t *> *p = &(this->pthreadList);
    return p;
}

//获取任务列表容器地址
ClassTaskPool* ClassPthreadMgr::GetTaskPool()
{
    return this->pTaskPool;
}

//添加线程地址入线程容器
void ClassPthreadMgr::AddPthread(pthread_t *tid)
{
    this->pthreadList.push_back(tid);
}

//获取任务参数
Task* ClassPthreadMgr::GetTaskArgs(int index)
{
    ClassTaskPool* taskPoolPtr = this->GetTaskPool();
    ClassTaskList* taskListPtr = taskPoolPtr->GetTaskListByID(index);

    //创建Task对象
    Task *task = new Task();
    task->pTaskList = taskListPtr;
    task->pMessList = &(taskListPtr->pMessTaskList);
    task->pWorkList = &(taskListPtr->pWorkTaskList);
    task->lock = &(taskListPtr->lock);
    task->cond = &(taskListPtr->cond);
    task->luaVmMgrPtr = this->luaVmMgrPtr;
    return task;
}

//把信息传进任务列表容器
void ClassPthreadMgr::AddMsgIntoTaskPool(list<MsgPackage*>& msgList,int minIndex)
{
    cout << "放入index为："<< minIndex << "的任务列表" << endl;
    ClassTaskList* pTaskList = this->pTaskPool->GetTaskListByID(minIndex);
    std::copy(msgList.begin(), msgList.end(), std::back_inserter(*(pTaskList->pMessTaskList)));

    //try_lock尝试判断pWorkTaskList是否已经空了
    int resTryLock = pthread_mutex_trylock(&(pTaskList->lock));
    if (resTryLock == 0)
    {
        pTaskList->SwapTaskList();
        pthread_mutex_unlock(&(pTaskList->lock)); //唤醒前先解锁，否则work线程被阻塞
        pthread_cond_signal(&(pTaskList->cond));
    }
    return;
}

int ClassPthreadMgr::CheckMinTaskList()
{
    //判断哪个任务列表最少任务并添加任务
    const int totalNum = Config::pollingPthreadNum;
    int minNum = this->pTaskPool->GetTaskListByID(0)->GetListSize();
    int minIndex = 0;
    for (int index = 0; index < totalNum; index++)
    {
        int listTaskNum = this->pTaskPool->GetTaskListByID(index)->GetListSize();
        if (minNum > listTaskNum)
        {
            minNum = listTaskNum;
            minIndex = index;
        }
    }
    return minIndex;
}

LuaVmMgr* ClassPthreadMgr::GetLuaVmMgrPtr()
{
    return this->luaVmMgrPtr;
}

//检测任务列表循环（用锁来获取资源防止线程争抢）
void *CheckTaskList(void *args)
{
    //线程参数
    ClassTaskList* pTaskList = ((Task*)args)->pTaskList;
    list<MsgPackage*>* pMessList = *(((Task*)args)->pMessList);
    list<MsgPackage*>* pWorkList = *(((Task*)args)->pWorkList);
    pthread_mutex_t *lock = ((Task *)args)->lock;
    pthread_cond_t *cond = ((Task *)args)->cond;
    LuaVmMgr* luaVmMgrPtr = ((Task*)args)->luaVmMgrPtr;
    pthread_t tid = pthread_self();

    //MessagePackage信息参数
    string stringMsg = "";  //CMD，玩家socket监听获取来的json数据
    std::string uid = "";   //调用者id
    std::string caller = "";//调用者,一开始调用者模块默认为空
    std::string called = "";//被调用的模块VM名字
    std::string fun = "";   //调用函数/协议
    std::string arg = "";   //参数

    pthread_mutex_lock(lock); //上锁
    while (true)
    {
        stringMsg.clear();
        if (pWorkList->size() < 1)
        {
            if(pMessList->size() < 1)
            { 
                //cout << "work任务容器与mess容器均为空,先解锁休眠并等待任务到来时候被唤醒" << endl;
                pthread_cond_wait(cond, lock);
            }
            else if(pMessList->size() >= 1)
            {
                //cout << "work任务容器为空，但mess任务容器不为空,直接交换容器执行任务" << endl;
                pTaskList->SwapTaskList();
            }
            pMessList = *(((Task*)args)->pMessList);
            pWorkList = *(((Task*)args)->pWorkList);
        }

        if (pWorkList->size() >= 1)
        {
            //从消息列表取出消息包并读取消息包信息
            MsgPackage* msgPtr = *(pWorkList->begin());
            pWorkList->erase(pWorkList->begin());
            stringMsg = msgPtr->GetCMD();

            
            //执行任务
            if (stringMsg.size() >= 1)
            {
                Json::Reader reader(Json::Features::strictMode());
                Json::Value parseData;
                if (reader.parse(stringMsg.c_str(), parseData))
                {
                    called = parseData.get("Moudle", 0).asString();
                    fun = parseData.get("Protocol", 0).asString();
                    caller = "";
                    arg = stringMsg;
                    uid = ((Client*)(msgPtr->GetOperatePtr()))->GetClientUid();
                    while (true)
                    {
                        std::cout << "caller " << caller << std::endl;
                        std::cout << "called " << called << std::endl;
                        std::cout << "fun " << fun << std::endl;
                        std::cout << "uid " << uid << std::endl;
                        std::cout << "arg " << arg << std::endl << endl;
                        //首先修正以下called的模块信息
                        auto it = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->GetMoudleInfo()->find(called);
                        if (it != luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->GetMoudleInfo()->end())
                        {
                            if (luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->judgeMoudleType(called) == 1)//个人模块
                            {
                                if (luaVmMgrPtr->CheckLuaVmIsExistByIndex(uid) == true)
                                {
                                    LuaBaseVm* vmPtr = luaVmMgrPtr->GetLuaVmByIndex(uid);
                                    lua_State* L = vmPtr->GetLuaStatePtr();
                                    std::cout << "调用个人模块虚拟机" << std::endl;
                                    
                                    //开始操作C++ Lua 交互栈
                                    lua_settop(L, 0);
                                    lua_getglobal(L, "Main_");
                                    lua_pushstring(L, uid.c_str());
                                    lua_pushstring(L, caller.c_str());
                                    lua_pushstring(L, called.c_str());
                                    lua_pushstring(L, fun.c_str());
                                    lua_pushstring(L, arg.c_str());
                                    lua_pcall(L, 5, 5, 0);

                                    if (lua_isnil(L, -1))
                                    {
                                        
                                        break;
                                    }
                                    else //需要传递到不同虚拟机
                                    {

                                    }
                                }
                                else
                                {
                                    std::cout << "Personal Lua VM IS Not Build :" << uid << endl;
                                    break;
                                }
                            }
                            else if(luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->judgeMoudleType(called) == 2)//公共模块
                            {
                                if (luaVmMgrPtr->CheckLuaVmIsExistByIndex(called) == true)
                                {
                                    LuaBaseVm* vmPtr = luaVmMgrPtr->GetLuaVmByIndex(called);
                                    lua_State* L = vmPtr->GetLuaStatePtr();
                                    std::cout << "调用公共模块虚拟机" << std::endl;

                                    //开始操作C++ Lua 交互栈
                                    lua_settop(L, 0);
                                    lua_getglobal(L, "Main_");
                                    lua_pushstring(L, uid.c_str());
                                    lua_pushstring(L, caller.c_str());
                                    lua_pushstring(L, called.c_str());
                                    lua_pushstring(L, fun.c_str());
                                    lua_pushstring(L, arg.c_str());
                                    lua_pcall(L, 5, 5, 0);

                                    if (lua_isnil(L, -1))
                                    {
                                        break;
                                    }
                                    else //需要传递到不同虚拟机
                                    {

                                    }
                                }
                                else
                                {
                                    std::cout << "Public Lua VM IS Not Build :" << called << endl;
                                    break;
                                }
                            }
                            else
                            {
                                std::cout << "Moudle Not Belong To Personal And Public" << called << endl;
                                break;
                            }
                        }
                        else
                        {
                            std::cout << "called Moudle Wrong With Finding " << called << endl;
                            break;
                        }
                    }
                }
            }

            //修改任务数量


            //销毁
            delete msgPtr;
            usleep(170000);
        }
    }
    //pthread_mutex_unlock(((Task*)args)->lock); //解锁,其实这一步解锁还是不解锁已经无所谓了,可以屏蔽掉
    return NULL;
}