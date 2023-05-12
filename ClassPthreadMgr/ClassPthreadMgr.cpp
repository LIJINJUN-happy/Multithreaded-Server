#include "ClassPthreadMgr.h"

using std::cout;
using std::string;
using std::list;
using std::endl;
using std::map;

//构造函数
ClassPthreadMgr::ClassPthreadMgr(LuaVmMgr* luaVmMgrP)
{
    //初始化变量
    if (Config::pollingPthreadNum < 1)
    {
        LOG.Log() << "线程数量不可以小于1" << endl;
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
        LOG.Log() << "Erro - 获取的线程数大于线程数！" << endl;
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
    task->dbPtr = nullptr;
    return task;
}

//把信息传进任务列表容器
void ClassPthreadMgr::AddMsgIntoTaskPool(list<MsgPackage*>& msgList,int minIndex)
{
    //LOG.Log() << "放入index为："<< minIndex << "的任务列表" << endl;
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
    ClassDataBase* dbPtr = ((Task*)args)->dbPtr;
    pthread_t tid = pthread_self();

    //MessagePackage信息参数
    string stringMsg = "";  //CMD，玩家socket监听获取来的json数据
    std::string uid = "";   //调用者id
    std::string caller = "";//调用者,一开始调用者模块默认为空
    std::string called = "";//被调用的模块VM名字
    std::string fun = "";   //调用函数/协议
    std::string arg = "";   //参数

    //是否用户操作
    bool userOperator = false;//默认为否,若用户类型协议则为true,用作任务数量减少依据

    pthread_mutex_lock(lock); //上锁
    while (true)
    {
        stringMsg.clear();
        if (pWorkList->size() < 1)
        {
            if(pMessList->size() < 1)
            { 
                //LOG.Log() << "work任务容器与mess容器均为空,先解锁休眠并等待任务到来时候被唤醒" << endl;
                pthread_cond_wait(cond, lock);
            }
            else if(pMessList->size() >= 1)
            {
                //LOG.Log() << "work任务容器为空，但mess任务容器不为空,直接交换容器执行任务" << endl;
                pTaskList->SwapTaskList();
            }
            pMessList = *(((Task*)args)->pMessList);
            pWorkList = *(((Task*)args)->pWorkList);
        }

        if (pWorkList->size() < 1)
        {
            continue;
        }
        //从消息列表取出消息包并读取消息包信息
        MsgPackage* msgPtr = *(pWorkList->begin());
        pWorkList->erase(pWorkList->begin());
        bool resCheck = msgPtr->CheckMsgType("Actor");
        if (resCheck == true)
        {
            //判断是否来自用户的socketMsg
            userOperator = true;
        }
        stringMsg = msgPtr->GetCMD();
        LOG.Log() << "stringMsg = " << stringMsg << std::endl;
            
        //执行任务
        if (stringMsg.size() >= 1)
        {

            Json::Reader reader(Json::Features::strictMode());
            Json::Value parseData;
            if (reader.parse(stringMsg.c_str(), parseData))
            {
                called = parseData.get("Moudle", 0).asString();
                fun = parseData.get("Protocol", 0).asString();

                //判断是不是登录或下线或者注册协议（需要注册过以及登录成功的用户才可以生成 ActorLua虚拟机）
                bool ifSkip = false;//是否跳过LuaVm的交互操作
                bool removeActorVmWithLogOut = false;
                if (called == "GATE" && fun == "c_login_request")//登录请求
                {
                    std::string account = parseData.get("Account", 0).asString();
                    std::string password = parseData.get("Password", 0).asString();
                    bool resultLogin = Gate::Login(account, password, dbPtr, msgPtr->GetOperatePtr(), luaVmMgrPtr, msgPtr->GetsockidMapPrt());
                    if (resultLogin == false) { ifSkip = true; }                                                                    //登录失败要调过虚拟机交互
                }
                else if (called == "GATE" && fun == "c_registered_request")//注册请求
                {
                    std::string account = parseData.get("Account", 0).asString();
                    std::string password = parseData.get("Password", 0).asString();
                    int code = parseData.get("Code", 0).asInt();
                    bool resRegistered = Gate::Registered(msgPtr->GetOperatePtr(), account, password, code, dbPtr);
                    ifSkip = true;                                                                                                  //注册必须跳过,因为只有登录成功才可进入对应的用户LuaVm进行交互
                }
                else if (called == "GATE" && fun == "c_registered_token_request")//注册码请求
                {
                    std::string em = parseData.get("EmailAddress", 0).asString();
                    bool resJudege = Gate::JudegeEmailBrandNew(em.c_str(), dbPtr, ((Client*)(msgPtr->GetOperatePtr()))->GetClientFd());
                    if (resJudege == true) { Gate::GetRegisteredToken(msgPtr->GetOperatePtr(), em.c_str()); }
                    ifSkip = true;                                                                                                  //注册码请求也需要跳过
                }
                else if (called == "GATE" && fun == "c_logout") { removeActorVmWithLogOut = true; }//下线

                caller = "";
                arg = stringMsg;
                uid = ((Client*)(msgPtr->GetOperatePtr()))->GetClientUid();
                while (ifSkip == false)
                {
                    LOG.Log() << "caller " << caller << std::endl;
                    LOG.Log() << "called " << called << std::endl;
                    LOG.Log() << "fun " << fun << std::endl;
                    LOG.Log() << "uid " << uid << std::endl;
                    LOG.Log() << "arg " << arg << std::endl;

                    //首先修正以下called的模块信息
                    auto it = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->GetMoudleInfo()->find(called);
                    if (it != luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->GetMoudleInfo()->end())
                    {
                        lua_State* L = nullptr;
                        pthread_mutex_t* vMLock = nullptr;
                        bool isCallPublicVm = false;

                        if (luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->judgeMoudleType(called) == 1)//个人模块
                        {
                            if (luaVmMgrPtr->CheckLuaVmIsExistByIndex(uid) == true)
                            {
                                LuaBaseVm* vmPtr = luaVmMgrPtr->GetLuaVmByIndex(uid);
                                L = vmPtr->GetLuaStatePtr();
                                LOG.Log() << "Call Personal LuaVm" << std::endl;
                            }
                            else
                            {
                                LOG.Log() << "Personal Lua VM IS Not Build :" << uid << endl;
                                break;
                            }
                        }
                        else if(luaVmMgrPtr->GetLuaMoudleFilesInfoPtr()->judgeMoudleType(called) == 2)//公共模块
                        {
                            if (luaVmMgrPtr->CheckLuaVmIsExistByIndex(called) == true)
                            {
                                //公共类Vm需要上锁
                                LuaBaseVm* vmPtr = luaVmMgrPtr->GetLuaVmByIndex(called);
                                vMLock = ((LuaPublicVm*)vmPtr)->GetPublickVmMutex();
                                pthread_mutex_lock(vMLock);
                                isCallPublicVm = true;
                                L = vmPtr->GetLuaStatePtr();
                                LOG.Log() << "Call Public LuaVm" << std::endl;
                            }
                            else
                            {
                                LOG.Log() << "Public Lua VM IS Not Build :" << called << endl;
                                break;
                            }
                        }
                        else
                        {
                            //LOG.Log() << "Moudle Not Belong To Personal And Public" << called << endl;
                            break;
                        }

                        //开始操作C++ Lua 交互栈
                        lua_settop(L, 0);
                        lua_getglobal(L, "Main_");
                        lua_pushstring(L, uid.c_str());
                        lua_pushstring(L, caller.c_str());
                        lua_pushstring(L, called.c_str());
                        lua_pushstring(L, fun.c_str());
                        lua_pushstring(L, arg.c_str());
                        lua_pcall(L, 5, 6, 0);

                        if (lua_isboolean(L,-6) && lua_toboolean(L,-6) == true)
                        {
                            if (lua_isnil(L, -1))
                            {
                                ifSkip = true;
                            }
                            else //需要传递到不同虚拟机
                            {
                                arg = lua_tostring(L, -5);
                                fun = lua_tostring(L, -4);
                                called = lua_tostring(L, -3);
                                caller = lua_tostring(L, -2);
                                uid = lua_tostring(L, -1);
                                LOG.Log() << "type -1 :" << lua_typename(L, lua_type(L, -1)) << std::endl;
                                LOG.Log() << "type -2 :" << lua_typename(L, lua_type(L, -2)) << std::endl;
                                LOG.Log() << "type -3 :" << lua_typename(L, lua_type(L, -3)) << std::endl;
                                LOG.Log() << "type -4 :" << lua_typename(L, lua_type(L, -4)) << std::endl;
                                LOG.Log() << "type -5 :" << lua_typename(L, lua_type(L, -5)) << std::endl << std::endl;
                            }
                        }
                        else
                        {
                            LOG.Log() << "lua_isboolean(L,-6) = " << lua_isboolean(L, -6) << endl;
                            LOG.Log() << "lua_toboolean(L,-6) = " << lua_toboolean(L, -6) << endl;
                            ifSkip = true;
                        }

                        //判断是否需要解锁（调用了共有Vm才需要解锁）
                        if (isCallPublicVm == true)
                        {
                            pthread_mutex_unlock(vMLock);
                        }
                    }
                    else
                    {
                        //LOG.Log() << "called Moudle Wrong With Finding " << called << endl;
                        break;
                    }
                }

                //用户下线（放在这里判断是为了让用户在LuaVm处理完下线操作后才进行移除）
                if (removeActorVmWithLogOut == true)
                {
                    Gate::RemoveFromSockIdMap(msgPtr->GetOperatePtr(),msgPtr->GetsockidMapPrt(),uid);//先移除SocketMap中的Client*
                    luaVmMgrPtr->DeleteLuaBaseVm(uid);                                               //再移除LuaVmMap中的Vm*
                }

            }
        }

        //修改任务数量
        if (userOperator == true && uid.size() > 0)
        {
            void* sockmapPtr = msgPtr->GetsockidMapPrt();
            auto it = ((map<string, Client*>*)sockmapPtr)->find(uid);
            if (it != ((map<string, Client*>*)sockmapPtr)->end())//防止掉线了,clientPtr指向无效对象，所以需要判断一下
            {
                Client* clientPtr = (Client*)(msgPtr->GetOperatePtr());
                clientPtr->UpdateClientTaskNum(-1);
                int taskNum = clientPtr->GetClientTaskNum();
                LOG.Log() << "Client " << uid << " Task Total Amount: " << taskNum << std::endl;
                if (taskNum <= 0)
                {
                    //恢复被操作的线程索引
                    clientPtr->UpdateWorkPthreadIndex(-1);
                }
            }
            else
            {
                //LOG.Log() << "Client " << "Is't Not Online" << std::endl;
            }
        }
        userOperator = false;

        //销毁
        delete msgPtr;
        //usleep(170000);
    }
    //pthread_mutex_unlock(((Task*)args)->lock); //解锁,其实这一步解锁还是不解锁已经无所谓了,可以屏蔽掉
    return NULL;
}