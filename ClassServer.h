#pragma once
#ifndef _CLASSSERVER_H_
#define _CLASSSERVER_H_

#include "./Net/ClassTcpNet.h"

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

	ClassTcpNet* tcpNetObj;

public:
    ClassServer();
	~ClassServer();
	ClassServer(ClassTcpNet* netObj);

public:
	int GetServerStatus();
	void SetServerStatus(int status);

	int GetServerConnectCondition();
	void SetServerConnectCondition(int condition);

	int GetActorAmount();
};


#endif