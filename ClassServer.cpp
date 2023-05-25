#include "ClassServer.h"

ClassServer::ClassServer()
{
}

ClassServer::~ClassServer()
{
}

ClassServer::ClassServer(ClassTcpNet* netObj)
{
	this->serverStatus = Global::Running;				//服务器状态
	this->serverConnectCondition = Global::Fluency;		//服务器连接情况
	this->serverStartTime = Global::GetNowTime();
	this->addrString = Config::addrString;
	this->listenPort = Config::listenPort;
	this->serverOpenTime = Config::serOpenTime;
	this->serverID = Config::servID;
	this->serverName = Config::servName;
	this->tcpNetObj = netObj;
}

int ClassServer::GetServerStatus()
{
	return this->serverStatus;
}

void ClassServer::SetServerStatus(int status)
{
	this->serverStatus = status;
	return;
}

int ClassServer::GetServerConnectCondition()
{
	return this->serverConnectCondition;
}

void ClassServer::SetServerConnectCondition(int condition)
{
	this->serverConnectCondition = condition;
	return;
}

int ClassServer::GetActorAmount()
{
	int num = this->tcpNetObj->GetSockidMap()->size();
	return num;
}
