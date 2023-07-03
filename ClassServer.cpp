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
	this->clientAmount = 0;
	this->minTaskListIndex = 1;
	this->noLimitDataListPtr = nullptr;
}

ClassServer::~ClassServer()
{
}

ClassServer::ClassServer(int epoll_fd)
{
	this->serverStatus = Global::Running;				//服务器状态
	this->serverConnectCondition = Global::Fluency;		//服务器连接情况
	this->serverStartTime = Global::GetNowTime();
	this->addrString = Config::addrString;
	this->listenPort = Config::listenPort;
	this->serverOpenTime = Config::serOpenTime;
	this->serverID = Config::servID;
	this->serverName = Config::servName;
	this->epollFd = epoll_fd;
	this->clientAmount = 0;
	this->minTaskListIndex = 1;
	this->noLimitDataListPtr = nullptr;
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
	return this->clientAmount.load();
}

void ClassServer::ChangeClientAmount(int val)
{
	if (val >= 0)
	{
		this->clientAmount.fetch_add(val);
		return;
	}
	if (val < 0)
	{
		val = abs(val);
		this->clientAmount.fetch_sub(val);
	}

	return;
}

void ClassServer::SetActorAmount(int val)
{
	this->clientAmount.store(val);
	return;
}

int ClassServer::GetEpollFd()
{
	return this->epollFd;
}

void ClassServer::SetEpollFd(int fd)
{
	this->epollFd = fd;
}

int ClassServer::GetMinTaskListIndex()
{
	return this->minTaskListIndex;
}

void* ClassServer::GetNoLimitDataListPtr()
{
	return this->noLimitDataListPtr;
}

void ClassServer::SetNoLimitDataListPtr(void* ptr)
{
	this->noLimitDataListPtr = ptr;
}

void ClassServer::SetMinTaskListIndex(int index)
{
	this->minTaskListIndex = index;
	return;
}
