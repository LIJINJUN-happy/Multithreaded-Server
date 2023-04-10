#include "ClassMsgPackage.h"

using std::cout;
using std::string;
using std::endl;

MsgPackage::MsgPackage(string cmdString, void *optr, void* mfdptr, void* midptr, const char* type)
{
	this->cmd = cmdString;
	this->operatePtr = optr;
	this->sockfdMapPrt = mfdptr;
	this->sockidMapPrt = midptr;
	//this->netObjectPrt = netObj;
	memset(msgType, 0, 0);
	strcpy(msgType,type);
}

MsgPackage::~MsgPackage()
{
}

string MsgPackage::GetCMD()
{
	return this->cmd;
}

bool MsgPackage::CheckMsgType(const char* type)
{
	int isSame = strcmp(msgType, type);
	if (isSame == 0)
	{
		return true;
	}
	return false;
}

void* MsgPackage::GetOperatePtr()
{
	void* p = nullptr;
	p = this->operatePtr;
	return p;
}

void* MsgPackage::GetsockfdMapPrt()
{
	return this->sockfdMapPrt;
}

void* MsgPackage::GetsockidMapPrt()
{
	return this->sockidMapPrt;
}

/*
void* MsgPackage::GetNetObjectPrt()
{
	return this->netObjectPrt;
}*/
