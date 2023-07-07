//离线数据处理模块
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------离线数据类(即使用户不在线,也可以从这里取出数据)
typedef struct OffLineData
{
private:
	std::map<std::string, std::string> offLineDataMap; //UID---JSON数据字符串
	std::map<std::string, std::string>* offLineDataMapPtr;

public:
	OffLineData();
	~OffLineData();

public:
	std::map<std::string, std::string>* GetOffLineDataMapPtr();	//返回离线数据容器地址
	bool CheckoutActorOffLineData(std::string uid);				//判断是否有该用户离线数据存在
	std::string& FindoutActorOffLineData(std::string uid);		//获取该用户离线数据的引用
	void AddData(std::string uid);								//创建离线数据
	void UpdateData(std::string uid, std::string jsonData);		//更新离线数据

} OffLineData;
//--------------------------END------离线数据类(即使用户不在线,也可以从这里取出数据)





//--------------------------BEGIN------登录待处理数据（用户登录的时候来这里查看）
typedef struct LoginOffLineMsg
{
private:
	std::map<std::string, std::string> loginOffLineMsgMap;		//UID---待处理数据字符串
	std::map<std::string, std::string>* loginOffLineMsgMapPtr;

public:
	LoginOffLineMsg();
	~LoginOffLineMsg();

public:
	std::map<std::string, std::string>* GetLoginOffLineMsgMapPtr();	//返回容器地址
	bool CheckLoginOffLineData(std::string uid);					//判断该用户是否有数据需要在登录后处理（用户不在线的是会存放至此,等上线的时候再处理）
	std::string& GetLoginOffLineData(std::string uid);				//获取该用户登录所需处理数据的引用
	void AddLoginOffLineData(std::string uid, std::string data);	//添加登录待处理数据

} LoginOffLineMsg;
//--------------------------END------登录待处理数据（用户登录的时候来这里查看）






//--------------------------BEGIN------离线系统
class OffLineSystem
{
public:
	OffLineSystem();
	~OffLineSystem();

public:
	OffLineData* GetOffLineDataPtr();
	LoginOffLineMsg* GetLoginOffLineMsgPtr();

private:
	OffLineData* OffLineDataPtr;
	LoginOffLineMsg* LoginOffLineMsgPtr;
};
//--------------------------END------离线系统

#endif-