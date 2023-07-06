#ifndef _CLASSTCPNET_H_
#define _CLASSTCPNET_H_

#include "../ClassPthreadMgr/ClassPthreadMgr.h"

void *epollListening(void *); //开始执行Epoll监听线程，把数据存进去Tasklist里面

class ClassTcpNet
{
private:
    std::map<std::string, Client*> pSockfdMap;  //套接字的容器（存放客户端套接字描述符）
    std::map<std::string, Client*> pSockidMap;  //用户UID的容器（存放客户端UID，注册登录完才可加载Client*进去）
    ClassPthreadMgr *pthreadObj;                //自定义线程类对象的指针（用来传递任务进入任务列表）
    int serverSock;                             //服务端监听套接字描述符
    int port;                                   //监听端口
    std::string addr;                           //地址字符串
    int maxConnect;                             //最大连接队列数
    int maxSocketfd;                            //最大socket对象连接数
    int maxEpollEvent;                          //最大epoll监视事件返回个数
    int epollfd;                                // Epoll_create函数返回的描述符

public:
    ClassTcpNet(ClassPthreadMgr *);      //构造函数
    ~ClassTcpNet();                      //析构函数
    void Init();                         //初始化（依次执行socket bind listen）
    void StartEpoll();                             //开始进入epoll循环监视
    std::map<std::string, Client*>* GetSockfdMap();//返回套接字容器地址
    std::map<std::string, Client*>* GetSockidMap();//返回UID容器地址
    void CloseClientByFd(std::string);             //根据fd关闭与某客户端套接字相关的任何信息

    bool CheckIsExistByUid(std::string uid);                     //根据用户uid判断是否存在pSockidMap中
    void AddClientIntoUidMap(std::string uid, Client* clientPtr);//添加用户进pSockidMap
    void RemoveClientByUid(std::string uid);                     //移除用户根据用户Uid

    void AddMsgIntoTaskPool(Client* pClient, std::list<MsgPackage*>& limitDataList, std::list<MsgPackage*>& noLimitDataList, int minTaskListIndex);

    int GetEpollFd();
    ClassPthreadMgr* GetPthreadObj();
};

#endif