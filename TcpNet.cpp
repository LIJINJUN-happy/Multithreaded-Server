#include "TcpNet.h"

//构造函数
TcpNet::TcpNet()
{
    this->pSockfdMap = new map<string,int>;
    this->port = Config::listenPort;
    this->addr = Config::addrString;
    this->maxConnect = Config::maxConnect;
    this->serverSock = -1;
    this->maxSocketfd = Config::maxSocketfd;
    this->maxEpollEvent = Config::maxEpollEvent;

    //创建Epoll（最大监听sockfd数量为Config::maxSocketfd配置值）
    this->epollfd = epoll_create(Config::maxSocketfd);
}

//析构函数
TcpNet::~TcpNet()
{
    delete this->pSockfdMap;
}

//初始化函数
void TcpNet::Init()
{
    //检测参数
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

    //开始执行socket bind listen
    this->serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSock == -1)
	{
		cout << "socket 函数创建套接字失败" << endl;
		return;
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
	int resultBind = bind(this->serverSock,(sockaddr*)&serverAddr, sizeof(serverAddr));
	if (resultBind == -1)
	{
		cout << "bind   函数绑定套接字失败" << endl;
		return;
	}
	else
	{
		cout << "bind   函数绑定套接字成功" << endl;
	}

	int resultListen = listen(this->serverSock, Config::maxConnect);
	if (resultBind == -1)
	{
		cout << "listen 函数监听失败" << endl;
		return;
	}
	else
	{
		cout << "正在监听端口：" << Config::listenPort << "................" << endl;
	}

    if(this->epollfd == -1) 
    {
        cout << "Epoll_create error ——————epoll创建失败" << endl;
        close(this->serverSock);
        return;
    }
    this->startEpoll();

	while (true)
	{
		struct sockaddr_in  clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		int clientSock = accept(this->serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSock == -1)
		{
			cout << "accept 函数接受客户端失败！" << endl;
		}
		else
		{
			cout << "accept 函数接受客户端成功！ clientSock = " << clientSock << endl;
            string key = to_string(clientSock);
			((*pSockfdMap)[key]) = clientSock;
			cout << "当前连接人数为：" << pSockfdMap->size() << endl;
		}
	}

    return;
}

//开始进入epoll循环监视
void TcpNet::startEpoll()
{
    const int eventsSize = Config::maxEpollEvent;
    struct epoll_event event, events[eventsSize];
    event.data.fd = this->serverSock;
    event.events = EPOLLIN;
    //开始监听服务端的sockfd监听套接字
    epoll_ctl(this->epollfd, EPOLL_CTL_ADD, this->serverSock, &event);
    
    //利用epoll_wait搭配while循环来获取监听结果
    while(true)
    {
        int resEpollwait = epoll_wait(this->epollfd, events, eventsSize, -1);//阻塞（timeout参数为-1）
        if (resEpollwait < 0)
        {
            cout << "epoll_wait出现了意外的错误!" << endl;
            break;
        }
        else if(resEpollwait == 0)
        {
            cout << "超时........." << endl;
            continue;
        }
        else
        {
            //接收到信息，开始循环读取文件描述符
            for(int total = 0; total < resEpollwait; total++)
            {
                if()
                {

                }
                else
                {
                    
                }
            }
        }
    }
    return ;
}

//开始执行Epoll监听线程，把数据存进去Tasklist里面
void* epollListening(void* args)
{
    ((TcpNet*)args)->Init();
    return NULL;
}