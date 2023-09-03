#include "ClassTcpNet.h"

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::list;

//epoll define
#define EPOLL_WAIT_ERRO -1          //EPOLL 错误
#define EPOLL_WAIT_TIME_OUT 0       //EPOLL 超时,监听的socket均没有数据变动
#define EPOLL_WAIT_ALREADY 1        //EPOLL 就绪的socket最少数量

//socket accept define
#define SOCKET_ACCEPT_FAIL -1       //SOCKET 接受客户端 错误

//socket read define
#define SOCKET_READ_FAIL -1       //SOCKET Read客户端数据 错误
#define SOCKET_READ_CLOSE 0       //SOCKET 客户端下线
#define SOCKET_READ_SUCCESS 1     //SOCKET Read客户端数据 正确


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
        LOG.Log() << "服务器监听地址不正确,长度不可以小于1" << endl;
        return;
    }

    if (this->port <= 0)
    {
        LOG.Log() << "服务器监听端口不正确,大小不可以小于1" << endl;
        return;
    }

    if (this->maxConnect <= 0)
    {
        LOG.Log() << "服务器最大连接数不正确,大小不可以小于1" << endl;
        return;
    }

    //开始执行socket bind listen
    this->serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSock == -1)
    {
        LOG.Log() << "\033[31msocket 函数创建套接字失败\033[0m" << endl;
        return;
    }
    else
    {
        LOG.Log() << "\033[32msocket 函数创建套接字成功\033[0m" << endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(Config::listenPort);
    serverAddr.sin_family = AF_INET;
    string addr = Config::addrString;
    serverAddr.sin_addr.s_addr = inet_addr(addr.data());
    int resultBind = bind(this->serverSock, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (resultBind == -1)
    {
        LOG.Log() << "\033[31mbind   函数绑定套接字失败\033[0m" << endl;
        return;
    }
    else
    {
        LOG.Log() << "\033[32mbind   函数绑定套接字成功\033[0m" << endl;
    }

    int resultListen = listen(this->serverSock, Config::maxConnect);
    if (resultBind == -1)
    {
        LOG.Log() << "\033[31mlisten 函数监听失败\033[0m" << endl;
        return;
    }
    else
    {
        LOG.Log() << "\033[32m正在监听端口: " << Config::listenPort << "\033[5m................\033[0m" << endl;
        LOG.Log() << "\033[35m监听线程启动步骤成功\033[0m\n"
             << endl;
    }

    if (this->epollfd == -1)
    {
        LOG.Log() << "Epoll_create error ——————epoll创建失败" << endl;
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
    list<MsgPackage*> noLimitDataList;
    list<MsgPackage*> limitDataList;
    char dataBuff[Config::maxReadDataSize] = "";

    extern ClassServer* SERVER_OBJECT;
    SERVER_OBJECT->SetNoLimitDataListPtr((void*)(&noLimitDataList));
    bool* isServerDowning = SERVER_OBJECT->GetIsServerDowning();//是否收到关服指令

    const int eventsSize = Config::maxEpollEvent;
    struct epoll_event eventServer;
    eventServer.data.fd = this->serverSock;
    eventServer.events = EPOLLIN | EPOLLET;
    epoll_ctl(this->epollfd, EPOLL_CTL_ADD, this->serverSock, &eventServer);//监听服务端的sockfd监听套接字

    //利用epoll_wait搭配while循环来获取监听结果
    while ( !(*isServerDowning) )
    {
        int minTaskListIndex = pthreadObj->CheckMinTaskList();
        SERVER_OBJECT->SetMinTaskListIndex(minTaskListIndex);
        struct epoll_event events[eventsSize];
        int resEpollwait = epoll_wait(this->epollfd, events, eventsSize, -1); //阻塞（timeout参数为-1）

        //处理Epoll结果
        if (resEpollwait <= EPOLL_WAIT_ERRO)
        {
            LOG.Log() << "epoll_wait出现了意外的错误!" << endl;
            break;
        }
        else if (resEpollwait == EPOLL_WAIT_TIME_OUT)
        {
            LOG.Log() << "超时........." << endl;
            continue;
        }
        else if(resEpollwait >= EPOLL_WAIT_ALREADY)
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
                    if (clientSock == SOCKET_ACCEPT_FAIL)
                    {
                        //LOG.Log() << "accept函数接受客户端失败!" << endl;
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
                        string key = std::to_string(clientSock);
                        string ipAddr = inet_ntoa(clientAddr.sin_addr);
                        (pSockfdMap[key]) = new Client(clientSock, key, ipAddr);
                        pSockfdMap[key]->UpdateHeartBeatTime();
                        extern ClassServer* SERVER_OBJECT;
                        SERVER_OBJECT->ChangeClientAmount(1);
                        //LOG.Log() << "Ser Accept Client Success ! ClientSocket = " << clientSock << endl;
                        //LOG.Log() << "当前连接人数为：" << pSockfdMap.size() << endl;
                    }
                }
                //否则是客户端的sockfd有信息
                else if (events[index].data.fd != this->serverSock && events[index].events == EPOLLIN)
                {
                    string fd = std::to_string(events[index].data.fd);
                    Client* pClient = pSockfdMap[fd]->GetMyself();

                    //需要一次性读取完,因为是边沿触发,所以用while来清空socket缓存
                    do{
                        memset(dataBuff, 0, 0);
                        int resRead = recv(events[index].data.fd, dataBuff, sizeof(dataBuff), MSG_DONTWAIT);
                        //客户端关闭了
                        if (resRead == SOCKET_READ_CLOSE)
                        {
                            //LOG.Log() << "Client:" << events[index].data.fd << "Close" << endl;
                            this->CloseClientByFd(std::to_string(events[index].data.fd));
                            std::string jsonMessage = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_logout\"}";
                            MsgPackage* msgPack = new MsgPackage(jsonMessage, (void*)pClient, (void*)(this->GetSockfdMap()), (void*)(this->GetSockidMap()), "Actor");
                            limitDataList.push_back(msgPack);
                            this->AddMsgIntoTaskPool(pClient, limitDataList, noLimitDataList, minTaskListIndex);
                            break;
                        }
                        //出错啦
                        else if (resRead <= SOCKET_READ_FAIL)
                        { 
                            if ((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))//这三个都是正常的错误,没有影响,用来判断缓存是否全部读取完了
                            {
                                this->AddMsgIntoTaskPool(pClient, limitDataList, noLimitDataList, minTaskListIndex);
                                break;
                            }
                            else
                            {
                                LOG.Log() << "客户端:" << events[index].data.fd << "出错了" << endl;
                                epoll_ctl(this->epollfd, EPOLL_CTL_DEL, events[index].data.fd, &(events[index]));
                                break;
                            }
                        }
                        //数据正确
                        else if (resRead >= SOCKET_READ_SUCCESS)
                        {
                            string messageResidue = pClient->GetMessageResidue();
                            messageResidue += string(dataBuff, 0, resRead);
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
                                    //LOG.Log() << "收到完整信息" << completeStr << endl;
                                    MsgPackage* msgPack = new MsgPackage(completeStr, (void*)pClient, (void*)(this->GetSockfdMap()), (void*)(this->GetSockidMap()), "Actor");
                                    limitDataList.push_back(msgPack);
                                    messageResidue.assign(messageResidue, findIndex + 1, messageResidue.npos);
                                    continue;
                                }
                            }
                            pClient->UpdateMessageResidue(messageResidue);
                        }
                    } while (true);
                    limitDataList.clear();
                }
            }
            if (noLimitDataList.size() > 0)
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

SafeMap<Client*>* ClassTcpNet::GetSockidMap()
{
    return &(this->pSockidMap);
}

//关闭与某客户端套接字相关的任何信息
void ClassTcpNet::CloseClientByFd(string fd)
{
    int clientFd = atoi(fd.c_str());
    close(clientFd);
    epoll_ctl(this->epollfd, EPOLL_CTL_DEL, clientFd, NULL);
    if(this->pSockfdMap.find(fd) != this->pSockfdMap.end())
    {
        this->pSockfdMap.erase(fd);
    }
    extern ClassServer* SERVER_OBJECT;
    SERVER_OBJECT->ChangeClientAmount(-1);
    //LOG.Log() << "当前SockfdMap人数为：" << pSockfdMap.size() << endl;
}

bool ClassTcpNet::CheckIsExistByUid(std::string uid)
{
    return pSockidMap.CheckoutIfExist(uid);
}

void ClassTcpNet::AddClientIntoUidMap(std::string uid, Client* clientPtr)
{
    if (pSockidMap.CheckoutIfExist(uid))
    {
        LOG.Log() << "Actor Has Existed,Need Not To Add " << std::endl;
        return;
    }
        
    this->pSockidMap.insert(uid, clientPtr);
    return;
}

void ClassTcpNet::RemoveClientByUid(std::string uid)
{
    if (pSockidMap.CheckoutIfExist(uid))
    {
        LOG.Log() << "Actor Has Not Existed,Need Not To Remove " << std::endl;
        return;
    }

    Client* pClient = this->pSockidMap.at(uid);
    this->pSockidMap.erase(uid);
    delete pClient;
    pClient = nullptr;
    //LOG.Log() << "当前pSockidMap人数为：" << pSockidMap.size() << endl;
    return;
}

void ClassTcpNet::AddMsgIntoTaskPool(Client* pClient, list<MsgPackage*>& limitDataList, list<MsgPackage*>& noLimitDataList, int minTaskListIndex)
{
    int workPthreadIndex = pClient->GetWorkPthreadIndex();
    int taskNum = pClient->GetClientTaskNum();
    //LOG.Log() << "workPthreadIndex = " << workPthreadIndex << endl;
    //LOG.Log() << "taskNum = " << taskNum << endl;
    if (taskNum <= 0)
    {   //证明该用户没有请求尚未完成
        int addSize = limitDataList.size();
        pClient->UpdateClientTaskNum(addSize);
        pClient->UpdateWorkPthreadIndex(minTaskListIndex);
        std::copy(limitDataList.begin(), limitDataList.end(), std::back_inserter(noLimitDataList));
    }
    else
    {   //还有任务正在任务链表中未处理完
        int addSize = limitDataList.size();
        pClient->UpdateClientTaskNum(addSize);
        this->pthreadObj->AddMsgIntoTaskPool(limitDataList, workPthreadIndex);
    }
}

int ClassTcpNet::GetEpollFd()
{
    return this->epollfd;
}

ClassPthreadMgr* ClassTcpNet::GetPthreadObj()
{
    return this->pthreadObj;
}

//开始执行Epoll监听线程，把数据存进去Tasklist里面
void *epollListening(void *args)
{
    ((ClassTcpNet *)args)->Init();
    return NULL;
}