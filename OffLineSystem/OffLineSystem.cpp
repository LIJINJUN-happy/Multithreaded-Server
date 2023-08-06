#include "./OffLineSystem.h"

OffLineData::OffLineData()
{
}

OffLineData::~OffLineData()
{
}

bool OffLineData::CheckoutActorOffLineData(std::string uid)
{
	return redisObj->checkoutData(uid + "_OFFLINE_DATA");
}

std::string OffLineData::FindoutActorOffLineData(std::string uid)
{
	std::string data = redisObj->get(uid + "_OFFLINE_DATA");
	return data;
}

void OffLineData::AddData(std::string uid)
{
	if (this->CheckoutActorOffLineData(uid) == true)
	{
		return;
	}

	std::string jsonData = "{}";
	redisObj->set(uid + "_OFFLINE_DATA", jsonData);
	return;
}

void OffLineData::UpdateData(std::string uid, std::string jsonData)
{
	if (this->CheckoutActorOffLineData(uid) != true)
	{
		return;
	}

	redisObj->set(uid + "_OFFLINE_DATA", jsonData);
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
	return it->second;;
}

void LoginOffLineMsg::UpdateLoginOffLineData(std::string uid, std::string data)
{
	this->loginOffLineMsgMap[uid] = data;
	return;
}

void LoginOffLineMsg::AddLoginOffLineData(std::string uid, std::string newdata)
{
	if (CheckLoginOffLineData(uid) == false)
	{
		this->loginOffLineMsgMap[uid] = newdata;
	}
	else
	{
		std::string& data = GetLoginOffLineData(uid);
		if (data.size() > 0)
		{
			data += "::";
		}
		data += newdata;
		UpdateLoginOffLineData(uid, data);
	}
	return;
}

std::string LoginOffLineMsg::GetData(std::string uid)
{
	std::string& data = GetLoginOffLineData(uid);
	std::string str(Global::BreakDownByString(data, "::"));

	//假如没有要处理的数据就直接删除掉元素
	if (data.size() == 0 || ((data.size() == 2) && (data == "::")))
	{
		if (loginOffLineMsgMap.find(uid) != loginOffLineMsgMap.end())
		{
			loginOffLineMsgMap.erase(uid);
		}
	}
	return str;
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
