#include "ClassTcpNet.h"

//构造函数
ClassTcpNet::ClassTcpNet(ClassPthreadMgr *p)
{
    this->port = Config::listenPort;
    this->addr = Config::addrString;
    this->maxConnect = Config::maxConnect;
    this->serverSock = -1;
    this->maxSocketfd = Config::maxSocketfd;
    this->maxEpollEvent = Config::maxEpollEvent;
    this->pthreadObj = p; //把线程对象的地址穿进来，可以调用它的类方法AddMsgIntoTaskPool存入任务

    //创建Epoll（最大监听sockfd数量为Config::maxSocketfd配置值）
    this->epollfd = epoll_create(Config::maxSocketfd);
}

//析构函数
ClassTcpNet::~ClassTcpNet()
{
}

//初始化函数
void ClassTcpNet::Init()
{
    //检测参数
    if (this->addr.size() <= 0)
    {
        cout << "服务器监听地址不正确,长度不可以小于1" << endl;
        return;
    }

    if (this->port <= 0)
    {
        cout << "服务器监听端口不正确,大小不可以小于1" << endl;
        return;
    }

    if (this->maxConnect <= 0)
    {
        cout << "服务器最大连接数不正确,大小不可以小于1" << endl;
        return;
    }

    //开始执行socket bind listen
    this->serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSock == -1)
    {
        cout << "\033[31msocket 函数创建套接字失败\033[0m" << endl;
        return;
    }
    else
    {
        cout << "\033[32msocket 函数创建套接字成功\033[0m" << endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(Config::listenPort);
    serverAddr.sin_family = AF_INET;
    string addr = Config::addrString;
    serverAddr.sin_addr.s_addr = inet_addr(addr.data());
    int resultBind = bind(this->serverSock, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (resultBind == -1)
    {
        cout << "\033[31mbind   函数绑定套接字失败\033[0m" << endl;
        return;
    }
    else
    {
        cout << "\033[32mbind   函数绑定套接字成功\033[0m" << endl;
    }

    int resultListen = listen(this->serverSock, Config::maxConnect);
    if (resultBind == -1)
    {
        cout << "\033[31mlisten 函数监听失败\033[0m" << endl;
        return;
    }
    else
    {
        cout << "\033[32m正在监听端口: " << Config::listenPort << "\033[5m................\033[0m" << endl;
        cout << "\033[35m监听线程启动步骤成功\033[0m\n"
             << endl;
    }

    if (this->epollfd == -1)
    {
        cout << "Epoll_create error ——————epoll创建失败" << endl;
        close(this->serverSock);
        return;
    }

    //开始使用epoll监视监听套接字
    this->StartEpoll();
    return;
}

//开始进入epoll循环监视
void ClassTcpNet::StartEpoll()
{
    char dataBuff[Config::maxReadDataSize] = "";
    list<string> noLimitDataList;
    list<string> limitDataList;
    const int eventsSize = Config::maxEpollEvent;
    struct epoll_event eventServer;
    eventServer.data.fd = this->serverSock;
    eventServer.events = EPOLLIN | EPOLLET;
    //开始监听服务端的sockfd监听套接字
    epoll_ctl(this->epollfd, EPOLL_CTL_ADD, this->serverSock, &eventServer);

    //利用epoll_wait搭配while循环来获取监听结果
    while (true)
    {
        int minTaskListIndex = pthreadObj->CheckMinTaskList();
        struct epoll_event events[eventsSize];
        int resEpollwait = epoll_wait(this->epollfd, events, eventsSize, -1); //阻塞（timeout参数为-1）
        if (resEpollwait < 0)
        {
            cout << "epoll_wait出现了意外的错误!" << endl;
            break;
        }
        else if (resEpollwait == 0)
        {
            cout << "超时........." << endl;
            continue;
        }
        else
        {
            //接收到信息，开始循环读取文件描述符
            for (int index = 0; index < resEpollwait; index++)
            {
                //假如是服务端的sockfd有信息
                if (events[index].data.fd == this->serverSock && events[index].events == EPOLLIN)
                {
                    struct sockaddr_in clientAddr;
                    socklen_t clientAddrSize = sizeof(clientAddr);
                    int clientSock = accept(this->serverSock, (sockaddr *)&clientAddr, &clientAddrSize); 
                    if (clientSock == -1)
                    {
                        cout << "accept函数接受客户端失败!" << endl;
                    }
                    else
                    {
                        //設置客戶端socket為非堵塞
                        //fcntl(clientSock, F_SETFL, fcntl(clientSock, F_GETFL, 0) | O_NONBLOCK);
                        int funResult = fcntl(clientSock, F_GETFL);
                        if (funResult < 0)
                        {
                            perror("-----------perror--------------");
                            continue;
                        }
                        if (fcntl(clientSock, F_SETFL, funResult | O_NONBLOCK) == -1)
                        {
                            perror("------------fcntl--------------");
                            continue;
                        }

                        struct epoll_event eventClient;
                        eventClient.data.fd = clientSock;
                        eventClient.events = EPOLLIN | EPOLLET;
                        epoll_ctl(this->epollfd, EPOLL_CTL_ADD, clientSock, &eventClient);
                        string key = to_string(clientSock);
                        string ipAddr = inet_ntoa(clientAddr.sin_addr);
                        (pSockfdMap[key]) = new Client(clientSock, key, ipAddr);
                        cout << "accept函数接受客户端成功! clientSock = " << clientSock << endl;
                        cout << "当前连接人数为：" << pSockfdMap.size() << endl;
                    }
                }
                //否则是客户端的sockfd有信息
                else if (events[index].data.fd != this->serverSock && events[index].events == EPOLLIN)
                {
                    string fd = to_string(events[index].data.fd);
                    Client* pClient = pSockfdMap[fd]->GetMyself();
                    string messageResidue = pClient->GetMessageResidue();

                    //需要一次性读取完,因为是边沿触发,所以用while来清空socket缓存
                    do{
                        memset(dataBuff, 0, 0);
                        int resRead = recv(events[index].data.fd, dataBuff, sizeof(dataBuff), MSG_DONTWAIT);
                        //客户多关闭了
                        if (resRead == 0)
                        {
                            cout << "客户端:" << events[index].data.fd << "关闭连接" << endl;
                            this->CloseClientByFd(to_string(events[index].data.fd));
                            break;
                        }
                        //出错啦
                        else if (resRead < 0)
                        { 
                            if ((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
                            {
                                //这三个都是正常的错误,没有影响,用来判断缓存是否全部读取完了
                                break;
                            }
                            else
                            {
                                cout << "客户端:" << events[index].data.fd << "出错了" << endl;
                                epoll_ctl(this->epollfd, EPOLL_CTL_DEL, events[index].data.fd, &(events[index]));
                                break;
                            }
                        }
                        //数据正确
                        else if (resRead >= 1)
                        {
                            messageResidue += dataBuff;
                            while (true)
                            {
                                int findIndex = messageResidue.find('|');
                                if (findIndex == string::npos)
                                {
                                    break;
                                }
                                else
                                {
                                    string completeStr(messageResidue, 0, findIndex);
                                    cout << "收到完整信息" << completeStr << endl;
                                    /*
                                    解析：待补充
                                    */
                                    limitDataList.push_back(completeStr);
                                    messageResidue.assign(messageResidue, findIndex + 1, messageResidue.npos);
                                    continue;
                                }
                            }
                        }
                    } while (true);
                    pClient->UpdateMessageResidue(messageResidue);

                    //判断是否有任务在某个线程中执行
                    atomic_int& workPthreadIndex = pClient->GetWorkPthreadIndex();
                    atomic_int& taskNum = pClient->GetClientTaskNum();
                    if (taskNum <= 0)
                    {
                        int addSize = limitDataList.size();
                        pClient->UpdateClientTaskNum(addSize);
                        pClient->UpdateWorkPthreadIndex(minTaskListIndex);
                        std::copy(limitDataList.begin(), limitDataList.end(), std::back_inserter(noLimitDataList));
                    }
                    else
                    {
                        int addSize = limitDataList.size();
                        pClient->UpdateClientTaskNum(addSize);
                        this->pthreadObj->AddMsgIntoTaskPool(limitDataList, workPthreadIndex);
                    }
                    limitDataList.clear();
                }
            }
            if (noLimitDataList.size > 0)
            {
                this->pthreadObj->AddMsgIntoTaskPool(noLimitDataList, minTaskListIndex);
                noLimitDataList.clear();
            }
        }
    }
    return;
}

//返回套接字容器地址
map<string, Client*> *ClassTcpNet::GetSockfdMap()
{
    return &(this->pSockfdMap);
}

//关闭与某客户端套接字相关的任何信息
void ClassTcpNet::CloseClientByFd(string fd)
{
    int clientFd = atoi(fd.c_str());
    close(clientFd);
    epoll_ctl(this->epollfd, EPOLL_CTL_DEL, clientFd, NULL);
    Client * pClient = pSockfdMap[fd]->GetMyself();
    delete pClient;
    this->pSockfdMap.erase(fd);
    cout << "当前连接人数为：" << pSockfdMap.size() << endl;
}

//开始执行Epoll监听线程，把数据存进去Tasklist里面
void *epollListening(void *args)
{
    ((ClassTcpNet *)args)->Init();
    return NULL;
}