//离线数据处理模块
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------离线数据类(即使用户不在线,也可以从这里取出数据)
typedef struct OffLineData
{
public:
	std::map<std::string, std::string> offLineDataMap; //UID---JSON数据字符串
	std::map<std::string, std::string>* offLineDataMapPtr;

public:
	OffLineData();
	~OffLineData();

	std::map<std::string, std::string>* GetOffLineDataMapPtr();	//返回离线数据容器地址
	bool CheckoutActorOffLineData(std::string uid);				//判断是否有该用户离线数据存在
	std::string& FindoutActorOffLineData(std::string uid);		//获取该用户离线数据的引用
	void AddData(std::string uid);

} OffLineData;
//--------------------------END------离线数据类(即使用户不在线,也可以从这里取出数据)





//--------------------------BEGIN------登录处理数据（用户登录的时候来这里查看）
typedef struct LoginOffLineMsg
{

} LoginOffLineMsg;
//--------------------------END------登录处理数据（用户登录的时候来这里查看）






//--------------------------BEGIN------离线系统
class OffLineSystem
{
public:


protected:

private:

};
//--------------------------END------离线系统

#endif-