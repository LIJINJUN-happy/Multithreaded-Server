#pragma once
#ifndef _CLASSSERVER_H_
#define _CLASSSERVER_H_

#include "OffLineSystem/OffLineSystem.h"

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

	int epollFd;									//����Socket
	std::atomic<int> clientAmount;					//�ͻ�������

	int minTaskListIndex;							//����ִ���߳�����Ӧ����������,�����������List������
	void *noLimitDataListPtr;						//�����ƽ���socket��Ϣ����ָ��

	Global::LuaMoudleFilesInfo* luaMoudleFilesInfoPtr;
	bool isServerDowning;							//�Ƿ����ڹرշ�������ֹͣ��¼�Լ�����Э�飩

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