#include "./OffLineSystem.h"

OffLineData::OffLineData()
{
}

OffLineData::~OffLineData()
{
}

bool OffLineData::CheckoutActorOffLineData(std::string uid)
{
	return false;
}

std::string OffLineData::FindoutActorOffLineData(std::string uid)
{
	std::string data = "";
	return data;
}

void OffLineData::AddData(std::string uid)
{
	if (this->CheckoutActorOffLineData(uid) == true)
	{
		return;
	}
	this->offLineDataMap[uid] = "";
}

void OffLineData::UpdateData(std::string uid, std::string jsonData)
{
	return;
}

LoginOffLineMsg::LoginOffLineMsg()
{
	this->loginOffLineMsgMap.clear();
	this->loginOffLineMsgMapPtr = &(this->loginOffLineMsgMap);
}

LoginOffLineMsg::~LoginOffLineMsg()
{
	return;
}

std::map<std::string, std::string>* LoginOffLineMsg::GetLoginOffLineMsgMapPtr()
{
	return this->loginOffLineMsgMapPtr;
}

bool LoginOffLineMsg::CheckLoginOffLineData(std::string uid)
{
	auto it = this->loginOffLineMsgMap.find(uid);
	if (it != this->loginOffLineMsgMap.end())
	{
		return true;
	}
	return false;
}

std::string& LoginOffLineMsg::GetLoginOffLineData(std::string uid)
{
	auto it = loginOffLineMsgMap.find(uid);
	std::string& data = it->second;
	return data;
}

void LoginOffLineMsg::AddLoginOffLineData(std::string uid, std::string data)
{
	this->loginOffLineMsgMap[uid] = data;
	return;
}

OffLineSystem::OffLineSystem()
{
	this->LoginOffLineMsgPtr = new LoginOffLineMsg();
	this->OffLineDataPtr = new OffLineData();
}

OffLineSystem::~OffLineSystem()
{
	return;
}

OffLineData* OffLineSystem::GetOffLineDataPtr()
{
	return this->OffLineDataPtr;
}

LoginOffLineMsg* OffLineSystem::GetLoginOffLineMsgPtr()
{
	return this->LoginOffLineMsgPtr;
}
