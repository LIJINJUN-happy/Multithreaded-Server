#ifndef _GATE_H_
#define _GATE_H_

#include "./Client.h"

//预声明
class Client;
class ClassTcpNet;

namespace Gate
{
	//转换为char*然后send（弃用）
	void TransformationAndSend(std::string msg);

	//接受recv的数据data然后分解里面有多少协议（弃用）
	std::vector<std::string> GetAndTransformation(std::string msg);

	//随机生成一个几位数(从from到to中生成times次随机数字并组合)
	int GetRandByTimes(int from, int to, int times);

	//判断邮箱是否无注册过账号
	bool JudegeEmailBrandNew(const char* tarEmailAddress, ClassDataBase* db);

	//注册码请求处理
	void GetRegisteredToken(void* cliptr, const char* tarEmailAddress);

	//注册请求处理
	bool Registered(void* cliptr, std::string account, std::string pw, int code);

	//登录请求处理
	bool Login(int fd, void* fdMapPtr, std::string account, std::string pw);

	//登录成功后创建用户LuaVm
	bool CreateLuaVmAfterLogin(void* cliptr, LuaVmMgr* luaVmMgrPtr);

}

#endif