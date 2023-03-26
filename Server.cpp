﻿#include "./ClassPthreadMgr/ClassPthreadMgr.h" //自定义线程头文件
#include "./Net/ClassTcpNet.h"			 //自定义TCP头文件
#include "./ClassTimer/ClassTimer.h"	 //自定义Timer头文件
#include "./Monitor/ClassMonitor.h"		 //自定义监视头文件

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


	//创建Lua文件模块类（用来索引以及判断模块类型）
	Global::LuaMoudleFilesInfo* luaMoudleFilesInfoPtr = new Global::LuaMoudleFilesInfo();
	luaMoudleFilesInfoPtr->LoadLuaMoudleFiles();


	//创建Lua模块管理系统（所有lua模块存放处）
	LuaVmMgr* luaVmMgrPtr = new LuaVmMgr(luaMoudleFilesInfoPtr);


	//启动多线程执行轮询任务列表
	ClassPthreadMgr *pthreadObj = new ClassPthreadMgr(luaVmMgrPtr);
	for (int i = 0; i < pthreadObj->GetPollingPthreadNum(); i++)
	{
		//每个工作线程对应一个任务链表,避免了多个工作线程争抢一个任务链表的情况
		Task* task = pthreadObj->GetTaskArgs(i);
		pthread_t tid = 0;
		int resulCreatePthread = pthread_create(&tid, NULL, CheckTaskList, task);
		if (resulCreatePthread == 0)
		{
			cout << "\033[32m第" << i + 1 << "个轮询线程: " << tid << "启动成功!\033[0m" << endl;
			pthreadObj->AddPthread(&tid);
		}
		else
		{
			cout << "\033[31m第" << i + 1 << "个线程启动失败!\033[0m" << endl;
			cout << "\033[31m线程启动步骤失败\033[0m" << endl;
			return -1;
		}
	}
	cout << "\033[35m任务列表轮询线程启动步骤成功\033[0m\n"
		 << endl;


	//创建计时器线程（精度是秒）
	ClassTimer *timeObj = new ClassTimer(Config::timerIntervalTime, pthreadObj);
	pthread_t timeTid = 0;
	int resTimerCreate = pthread_create(&timeTid, NULL, TimerLooping, timeObj);
	if (resTimerCreate == 0)
	{
		cout << "\033[32m计时器精度间隔为: " << timeObj->GetIntervalTime() << "秒\033[0m" << endl;
		cout << "\033[35m计时器线程启动步骤成功\033[0m\n"
			 << endl;
	}
	else
	{
		cout << "\033[31m计时器线程启动步骤失败\033[0m" << endl;
		return -1;
	}


	//创建socketObj监听线程
	ClassTcpNet *tcpNetObj = new ClassTcpNet(pthreadObj);
	pthread_t netTid = 0;
	int resListen = pthread_create(&netTid, NULL, epollListening, tcpNetObj);
	if (resListen == 0)
	{
		;
	}
	else
	{
		cout << "\033[31m监听线程启动步骤失败\033[0m" << endl;
		return -1;
	}


	//监视循环
	ClassMonitor *monitorObj = new ClassMonitor(tcpNetObj, pthreadObj);
	monitorObj->BeginCheck();

	return 0;
}
