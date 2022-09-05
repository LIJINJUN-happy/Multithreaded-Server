/*此为一个单进程服务器，一个定时器线程，一个epoll线程（获取最新动态），6个轮询任务列表线程，以及主监听线程     */
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ClassPthread.h"  //线程头文件

using namespace std;
int main()
{
	//1.创建套接字列表，任务列表以及线程列表
	vector<int>* pSockfdList = new vector<int>;
	 
	//2.启动各种线程
	ClassPthread * pthreadObj = new ClassPthread();
	pthreadObj->Start();
 

	//3.创建socket监听
	int serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == -1)
	{
		cout << "socket 函数创建套接字失败" << endl;
		return 0;
	}
	else
	{
		cout << "socket 函数创建套接字成功" << endl;
	}
	
	struct sockaddr_in  serverAddr;
	serverAddr.sin_port = htons(Config::listenPort);
	serverAddr.sin_family = AF_INET;
	string addr = Config::addrString;
	serverAddr.sin_addr.s_addr = inet_addr(addr.data());
	int resultBind = bind(serverSock,(sockaddr*)&serverAddr, sizeof(serverAddr));
	if (resultBind == -1)
	{
		cout << "bind   函数绑定套接字失败" << endl;
		return 0;
	}
	else
	{
		cout << "bind   函数绑定套接字成功" << endl;
	}

	int resultListen = listen(serverSock, Config::maxConnect);
	if (resultBind == -1)
	{
		cout << "listen 函数监听失败" << endl;
		return 0;
	}
	else
	{
		cout << "正在监听端口：" << Config::listenPort << "   ................" << endl;
	}

	while (true)
	{
		struct sockaddr_in  clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSock == -1)
		{
			cout << "accept 函数接受客户端失败！" << endl;
		}
		else
		{
			cout << "accept 函数接受客户端成功！" << endl;
			(*pSockfdList).push_back(clientSock);
			cout << "当前连接人数为：" << pSockfdList->size() << endl;
		}
	}
}
