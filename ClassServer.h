#pragma once
#ifndef _CLASSSERVER_H_
#define _CLASSSERVER_H_

#include "./GlobalVariable.h"

class ClassServer
{
private:
	std::string serverName;							//����������
	int serverID;									//������ID
	long serverOpenTime;							//����������ʱ��
	int listenPort;     							//�����˿ں�(ȷ���ö˿ڿ�ͨ�˷���ǽ)
	std::string addrString;							//��ַ

	long serverStartTime;							//����������ʱ��
	int serverStatus;								//������״̬
	int serverConnectCondition;						//�������������

	int epollFd;								//����Socket
	std::atomic<int> clientAmount;				//�ͻ�������

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
};


#endif