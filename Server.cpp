﻿#include "./ClassPthreadMgr/ClassPthreadMgr.h" //自定义线程头文件
#include "./Net/ClassTcpNet.h"			 //自定义TCP头文件
#include "./ClassTimer/ClassTimer.h"	 //自定义Timer头文件
#include "./Monitor/ClassMonitor.h"		 //自定义监视头文件

//全局变量
SafeMap<int> GLOBAL_UID_SOCKET_MAP = {};				//UID - SOCKET 键值对容器
ClassServer *SERVER_OBJECT = nullptr;					//服务器类对象指针

//lua虚拟机所注册的定时任务,放在这里,然后定时器从这里取出
std::list<std::string> TIMER_LIST = {};					//定时器容器
pthread_mutex_t TIMER_LIST_LOCK;						//定时器容器锁（防止塞入或取出任务同时操作）

//Redis容器（Uid -- RedisObject Address）
SafeMap<Redis*> GLOBAL_UID_REDISOBJECT_MAP = {};

//数据库管理对象
DataBaseMgr DATABASEMGR;

using namespace std;
int main()
{
	//创建子进程
	switch (fork())
	{
	case -1:
		//创建子进程失败，这里可以写进日志
		return -1;

	case 0:
		//子进程返回
		break;

	default:
		//父进程返回，直接退出
		exit(0);
	}

	if (setsid() == -1) //新建会话
	{
		return -1;
	}
	chdir("/"); //改变当前工作目录
	umask(0);	//防止限制文件权限引起混乱


	//日志初始化
	LOG.Init();


	//数据库链接
	DATABASEMGR.Start();
	if (DATABASEMGR.DoLoadOffLineData() == false)
	{
		return -1;
	}
	LOG.Log() << "\033[35mHost = "<< Config::host <<" Port = "<< Config::port << "\033[0m\n";
	LOG.Log() << "\033[35mDB模块连接数据库成功！\033[0m\n" << endl;


	//创建Lua文件模块类（用来索引以及判断模块类型）
	Global::LuaMoudleFilesInfo* luaMoudleFilesInfoPtr = new Global::LuaMoudleFilesInfo();
	luaMoudleFilesInfoPtr->LoadLuaMoudleFiles();
	DATABASEMGR.SetLuaMoudleFilesInfoPtr(luaMoudleFilesInfoPtr);


	//创建Lua模块管理系统（所有lua模块存放处）
	LOG.Log() << "\033[35m预加载Lua公共模块\033[0m\n";
	LuaVmMgr* luaVmMgrPtr = new LuaVmMgr(luaMoudleFilesInfoPtr);
	bool resLoadMoudle = luaVmMgrPtr->InitPublicMoudle();
	if (!resLoadMoudle)
	{
		LOG.Log() << "\033[31mLua模块管理系统加载模块失败！\033[0m" << endl;
		return -1;
	}
	else
	{
		//创建并初始化好公共模块后，加载各个公共模块数据
		bool loadData = DATABASEMGR.LoadAllPublicData();
		if (loadData == false)
		{
			LOG.Log() << "\033[31mLua公共模块加载数据失败！\033[0m" << endl;
			return -1;
		}
		LOG.Log() << "\033[35mLua模块管理系统加载模块成功！\033[0m\n" << endl;
	}


	//启动多线程执行轮询任务列表
	ClassPthreadMgr *pthreadObj = new ClassPthreadMgr(luaVmMgrPtr);
	for (int i = 0; i < pthreadObj->GetPollingPthreadNum(); i++)
	{
		//每个工作线程对应一个任务链表,避免了多个工作线程争抢一个任务链表的情况
		Task* task = pthreadObj->GetTaskArgs(i);
		task->dbPtr = DATABASEMGR.GetDBByIndex(i);
		pthread_t tid = 0;
		int resulCreatePthread = pthread_create(&tid, NULL, CheckTaskList, task);
		if (resulCreatePthread == 0)
		{
			LOG.Log() << "\033[32m第" << i + 1 << "个轮询线程: " << tid << "启动成功!\033[0m" << endl;
			pthreadObj->AddPthread(&tid);
		}
		else
		{
			LOG.Log() << "\033[31m第" << i + 1 << "个线程启动失败!\033[0m" << endl;
			LOG.Log() << "\033[31m线程启动步骤失败\033[0m" << endl;
			return -1;
		}
	}
	LOG.Log() << "\033[35m任务列表轮询线程启动步骤成功\033[0m\n"
		 << endl;


	//创建socketObj监听线程
	SERVER_OBJECT = new ClassServer();//初始化Server类对象;
	ClassTcpNet *tcpNetObj = new ClassTcpNet(pthreadObj);
	pthread_t netTid = 0;
	int resListen = pthread_create(&netTid, NULL, epollListening, tcpNetObj);
	if (resListen == 0)
	{
		int epoll_fd = tcpNetObj->GetEpollFd();
		SERVER_OBJECT->SetEpollFd(epoll_fd);
		SERVER_OBJECT->SetLuaMoudleFilesInfoPtr(luaMoudleFilesInfoPtr);
	}
	else
	{
		LOG.Log() << "\033[31m监听线程启动步骤失败\033[0m" << endl;
		return -1;
	}


	//创建计时器线程（精度是秒）
	pthread_mutex_init(&TIMER_LIST_LOCK, NULL);	//初始化定时器容器锁
	ClassTimer* timeObj = new ClassTimer(Config::timerIntervalTime, tcpNetObj);
	pthread_t timeTid = 0;
	int resTimerCreate = pthread_create(&timeTid, NULL, TimerLooping, timeObj);
	if (resTimerCreate == 0)
	{
		LOG.Log() << "\033[32m计时器精度间隔为: " << timeObj->GetIntervalTime() << "秒\033[0m" << endl;
		LOG.Log() << "\033[35m计时器线程启动步骤成功\033[0m\n"
			<< endl;
	}
	else
	{
		LOG.Log() << "\033[31m计时器线程启动步骤失败\033[0m" << endl;
		return -1;
	}


	//监视循环
	Gate::SignalReady();
	ClassMonitor *monitorObj = new ClassMonitor(tcpNetObj, pthreadObj);
	monitorObj->BeginCheck();

	return 0;
}
