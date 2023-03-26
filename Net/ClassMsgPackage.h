#ifndef _CLASSMSGPACKAGE_H_
#define _CLASSMSGPACKAGE_H_

#include "../LuaServer/LuaVmMgr/ClassLuaVmMgr.h"

//��Ϣ����

/*
msgType :

1.NetRecv

2.System

3.Call

4.CallBack

5.NetSend

*/

class Client;//Ԥ����

class MsgPackage
{
public:
	MsgPackage(std::string cmdString, void* optr, void* mptr, const char* type);
	~MsgPackage();

	std::string GetCMD();
	bool CheckMsgType(const char*);
	void* GetOperatePtr();

private:
	std::string cmd;	//�������Ϣ��json�ַ�����ʽ��
	void* operatePtr;	//ִ���ߵ�ָ�루�п������û�����Ϣ���п�����ϵͳ��Ϣ����Ҫ�Լ�ת����
	void* sockfdMapPrt;	//ָ��ͻ���SocketMap��������ַ(����operatePtrΪ�û�����˳�Աһ����ΪNullptr)
	char msgType[10];	//��Ϣ����
};	

#endif