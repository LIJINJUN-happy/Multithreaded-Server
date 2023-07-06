#ifndef _CLASSMSGPACKAGE_H_
#define _CLASSMSGPACKAGE_H_

#include "../ClassDB/DataBaseMgr.h"

class Client;//Ԥ����

class MsgPackage
{
public:
	MsgPackage(std::string cmdString, void* optr, void* mfdptr, void* midptr, const char* type);
	~MsgPackage();
	MsgPackage() {};

	std::string GetCMD();
	bool CheckMsgType(const char*);
	void* GetOperatePtr();
	void* GetsockfdMapPrt();
	void* GetsockidMapPrt();
	std::string GetMsgType();
	//void* GetNetObjectPrt();

private:
	std::string cmd;	//�������Ϣ��json�ַ�����ʽ��
	void* operatePtr;	//ִ���ߵ�ָ�루�п������û�����Ϣ���п�����ϵͳ��Ϣ����Ҫ�Լ�ת����
	void* sockfdMapPrt;	//ClassTcpNet������ԱSocketfdMap��������ַ
	void* sockidMapPrt;	//ClassTcpNet������ԱSocketidMap��������ַ
	//void* netObjectPrt;	//ָ��ClassTcpNet�����
	char msgType[10];	//��Ϣ����
};	

#endif