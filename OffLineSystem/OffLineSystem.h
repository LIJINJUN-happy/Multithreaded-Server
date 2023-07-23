//离线数据处理模块
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------离线数据类(即使用户不在线,也可以从这里取出数据)
typedef struct OffLineData
{
public:

	OffLineData();

	~OffLineData();

	//判断是否有该用户离线数据存在
	bool CheckoutActorOffLineData(std::string uid);

	//获取该用户离线数据
	std::string FindoutActorOffLineData(std::string uid);

	//创建离线数据
	void AddData(std::string uid);

	//更新离线数据
	void UpdateData(std::string uid, std::string jsonData);

private:
	Redis* redisObj;	//redis对象（加载和存储离线数据）

public:

	//从Mysql加载离线数据进Redis
	void DoLoad();

	//从Redis取出离线数据存储进Mysql
	void DoSave();

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

	//返回容器地址
	std::map<std::string, std::string>* GetLoginOffLineMsgMapPtr();

	//判断该用户是否有数据需要在登录后处理（用户不在线的是会存放至此,等上线的时候再处理）
	bool CheckLoginOffLineData(std::string uid);

	//获取该用户登录所需处理数据的引用
	std::string& GetLoginOffLineData(std::string uid);

	//刷新登录待处理数据
	void UpdateLoginOffLineData(std::string uid, std::string data);	

	//新增登录待处理数据
	void AddLoginOffLineData(std::string uid, std::string data);

	//返回并删除待处理数据中的第一条数据
	std::string GetData(std::string uid);

} LoginOffLineMsg;
//--------------------------END------登录待处理数据（用户登录的时候来这里查看）






//--------------------------BEGIN------离线系统
class OffLineSystem
{
public:
	OffLineSystem();

	~OffLineSystem();

public:

	//获取离线管理对象地址
	OffLineData* GetOffLineDataPtr();

	//获取登录待处理数据管理对象地址
	LoginOffLineMsg* GetLoginOffLineMsgPtr();

private:
	OffLineData* OffLineDataPtr;
	LoginOffLineMsg* LoginOffLineMsgPtr;
};
//--------------------------END------离线系统

#endif