#ifndef _CLASSMSGPACKAGE_H_
#define _CLASSMSGPACKAGE_H_

#include "../LuaServer/LuaVmMgr/ClassLuaVmMgr.h"

//消息包类

/*
msgType :

1.NetRecv

2.System

3.Call

4.CallBack

5.NetSend

*/

class Client;//预声明

class MsgPackage
{
public:
	MsgPackage(std::string cmdString, void* optr, void* mptr, const char* type);
	~MsgPackage();

	std::string GetCMD();
	bool CheckMsgType(const char*);
	void* GetOperatePtr();

private:
	std::string cmd;	//处理的消息（json字符串格式）
	void* operatePtr;	//执行者的指针（有可能是用户的信息，有可能是系统消息，需要自己转换）
	void* sockfdMapPrt;	//指向客户端SocketMap的容器地址(假如operatePtr为用户，则此成员一般设为Nullptr)
	char msgType[10];	//消息类型
};	

#endif