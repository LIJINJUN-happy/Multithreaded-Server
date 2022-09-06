#include "TcpNet.h"

TcpNet::TcpNet()
{
    this->pSockfdList = new vector<int>;
    this->port = Config::listenPort;
    this->addr = Config::addrString;
    this->maxConnect = Config::maxConnect;
}

TcpNet::~TcpNet()
{
    delete this->pSockfdList;
}

bool TcpNet::Init()
{
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == -1)
	{
		cout << "socket 函数创建套接字失败" << endl;
		return false;
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
		return false;
	}
	else
	{
		cout << "bind   函数绑定套接字成功" << endl;
	}

	int resultListen = listen(serverSock, Config::maxConnect);
	if (resultBind == -1)
	{
		cout << "listen 函数监听失败" << endl;
		return false;
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

    return true;
}

void TcpNet::Start()
{
    if( this->addr.size() <= 0)
    {
        cout << "服务器监听地址不正确,长度不可以小于1" << endl;
        return ;
    }

    if( this->port <= 0)
    {
        cout << "服务器监听端口不正确,大小不可以小于1" << endl;
        return ;
    }

    if( this->maxConnect <= 0)
    {
        cout << "服务器最大连接数不正确,大小不可以小于1" << endl;
        return ;
    }

    bool result = this->Init();
    if (result == false)
    {
        cout << "网络监听步骤启动失败" << endl;
    }
    else
    {
        ;
    }
}

void TcpNet::Stop()
{

}