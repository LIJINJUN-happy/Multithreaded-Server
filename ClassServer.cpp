#include "ClassServer.h"

ClassServer::ClassServer()
{
	this->serverStatus = Global::Running;				//服务器状态
	this->serverConnectCondition = Global::Fluency;		//服务器连接情况
	this->serverStartTime = Global::GetNowTime();
	this->addrString = Config::addrString;
	this->listenPort = Config::listenPort;
	this->serverOpenTime = Config::serOpenTime;
	this->serverID = Config::servID;
	this->serverName = Config::servName;
}

ClassServer::~ClassServer()
{
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
	return 0;
}
