#ifndef _CLASSMSGPACKAGE_H_
#define _CLASSMSGPACKAGE_H_

#include "../ClassDB/DataBaseMgr.h"

class Client;//预声明

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
	std::string cmd;	//处理的消息（json字符串格式）
	void* operatePtr;	//执行者的指针（有可能是用户的信息，有可能是系统消息，需要自己转换）
	void* sockfdMapPrt;	//ClassTcpNet类对象成员SocketfdMap的容器地址
	void* sockidMapPrt;	//ClassTcpNet类对象成员SocketidMap的容器地址
	//void* netObjectPrt;	//指向ClassTcpNet类对象
	char msgType[10];	//消息类型
};	

#endif