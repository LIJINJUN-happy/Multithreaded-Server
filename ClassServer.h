#pragma once
#ifndef _CLASSSERVER_H_
#define _CLASSSERVER_H_

#include "OffLineSystem/OffLineSystem.h"

class ClassServer
{
private:
	std::string serverName;							//服务器名字
	int serverID;									//服务器ID
	long serverOpenTime;							//服务器开服时间
	int listenPort;     							//监听端口号(确保该端口开通了防火墙)
	std::string addrString;							//地址

	long serverStartTime;							//服务器启动时间
	int serverStatus;								//服务器状态
	int serverConnectCondition;						//服务器连接情况

	int epollFd;									//监听Socket
	std::atomic<int> clientAmount;					//客户端数量

	int minTaskListIndex;							//所有执行线程所对应的任务链中,存放最少任务List的索引
	void *noLimitDataListPtr;						//无限制接受socket信息容器指针

	Global::LuaMoudleFilesInfo* luaMoudleFilesInfoPtr;
	bool isServerDowning;							//是否正在关闭服务器（停止登录以及接受协议）

public:
    ClassServer();

	~ClassServer();

	ClassServer(int epoll_fd);

public:
	int GetServerStatus();

	void SetServerStatus(int status);

	int GetServerConnectCondition();

	void SetServerConnectCondition(int condition);

	int GetActorAmount();

	void ChangeClientAmount(int val);

	void SetActorAmount(int val);

	int GetEpollFd();

	void SetEpollFd(int fd);

	int GetMinTaskListIndex();

	void SetMinTaskListIndex(int index);

	void* GetNoLimitDataListPtr();

	void SetNoLimitDataListPtr(void * ptr);

	void SetLuaMoudleFilesInfoPtr(Global::LuaMoudleFilesInfo* ptr);

	Global::LuaMoudleFilesInfo* GetLuaMoudleFilesInfoPtr();

	void SetIsServerDowning(bool isStop);

	bool* GetIsServerDowning();
};


#endif