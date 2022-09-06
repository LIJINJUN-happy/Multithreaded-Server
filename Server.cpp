/*此为一个单进程服务器，一个定时器线程，一个epoll线程（获取最新动态），多个轮询任务列表线程，以及一个监听线程     */
#include "ClassPthread.h"  //线程头文件
#include "TcpNet.h"

using namespace std;
int main()
{	 
	//启动多线程执行轮询任务列表
	ClassPthread * pthreadObj = new ClassPthread();
	pthreadObj->Start();
 

	//创建socketObj监听
	TcpNet * tcpNetObj = new TcpNet();
	tcpNetObj->Start();
}
