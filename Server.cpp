#include "./ClassPthread/ClassPthread.h" //自定义线程头文件
#include "./Net/ClassTcpNet.h"			 //自定义TCP头文件
#include "./ClassTimer/ClassTimer.h"	 //自定义Timer头文件

using namespace std;
int main()
{
	//启动多线程执行轮询任务列表
	ClassPthread *pthreadObj = new ClassPthread();
	Task task = pthreadObj->GetTaskArgs();
	for (int i = 0; i < pthreadObj->GetPollingPthreadNum(); i++)
	{
		pthread_t tid = 0;
		int resulCreatePthread = pthread_create(&tid, NULL, CheckTaskList, &task);
		if (resulCreatePthread == 0)
		{
			cout << "第" << i + 1 << "个轮询线程: " << tid << "启动成功!" << endl;
			pthreadObj->AddPthread(&tid);
		}
		else
		{
			cout << "第" << i + 1 << "个线程启动失败!" << endl;
			cout << "线程启动步骤失败" << endl;
			return -1;
		}
	}
	cout << "任务列表轮询线程启动步骤成功\n"
		 << endl;

	//创建计时器线程（精度是秒）
	ClassTimer *timeObj = new ClassTimer(Config::timerIntervalTime, pthreadObj);
	pthread_t timeTid = 0;
	int resTimerCreate = pthread_create(&timeTid, NULL, TimerLooping, timeObj);
	if (resTimerCreate == 0)
	{
		cout << "计时器线程启动步骤成功\n"
			 << endl;
	}
	else
	{
		cout << "计时器线程启动步骤失败" << endl;
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
		cout << "监听线程启动步骤失败" << endl;
		return -1;
	}

	while (1)
	{
	}

	return 0;
}
