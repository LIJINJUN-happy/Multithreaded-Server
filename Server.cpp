/*此为一个单进程服务器，一个定时器线程，一个epoll线程（获取最新动态，且监听端口），多个轮询任务列表线程  */
#include "./ClassPthread/ClassPthread.h" //自定义线程头文件
#include "./ClassTcpNet/ClassTcpNet.h"  //自定义TCP头文件

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

	//创建socketObj监听线程
	ClassTcpNet *tcpNetObj = new ClassTcpNet(pthreadObj);
	pthread_t listen_Tid = 0;
	int resListen = pthread_create(&listen_Tid, NULL, epollListening, tcpNetObj);
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
