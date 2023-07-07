#include "./OffLineSystem.h"

OffLineData::OffLineData()
{
	this->offLineDataMap.clear();
	this->offLineDataMapPtr = &(this->offLineDataMapPtr);
}

OffLineData::~OffLineData()
{
}

std::map<std::string, std::string>* OffLineData::GetOffLineDataMapPtr()
{
	return this->offLineDataMapPtr;
}

bool OffLineData::CheckoutActorOffLineData(std::string uid)
{
	auto it = offLineDataMap.find(uid);
	if (it != offLineDataMap.end())
	{
		return true;
	}
	return false;
}

std::string& OffLineData::FindoutActorOffLineData(std::string uid)
{
	auto it = offLineDataMap.find(uid);
	std::string& data = it->second;
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