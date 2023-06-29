#ifndef _GATE_H_
#define _GATE_H_

#include "./Client.h"

//预声明
class Client;
class ClassTcpNet;

extern std::map<std::string, int> GLOBAL_UID_SOCKET_MAP;

namespace Gate
{
	//转换为char*然后send（弃用）
	void TransformationAndSend(std::string msg);

	//接受recv的数据data然后分解里面有多少协议（弃用）
	std::vector<std::string> GetAndTransformation(std::string msg);

	//随机生成一个几位数(从from到to中生成times次随机数字并组合)
	int GetRandByTimes(int from, int to, int times);

	//判断邮箱是否无注册过账号
	bool JudegeEmailBrandNew(const char* tarEmailAddress, ClassDataBase* db, int socket);

	//注册码请求处理
	bool GetRegisteredToken(void* cliptr, const char* tarEmailAddress);

	//注册请求处理
	bool Registered(void* cliptr, std::string account, std::string pw, int code, ClassDataBase* db);

	//登录验证账号密码(返回玩家UID,验证失败返回空字符)
	std::string CheckoutAccountPassword(std::string account, std::string pw, ClassDataBase* db);

	//登录请求处理
	bool Login(std::string account, std::string pw, ClassDataBase* db, void* cliptr, LuaVmMgr* luaVmMgrPtr, void* sockidmapPtr, void* sockfdmapPtr, ClassServer* classServerPtr);

	//登录成功后创建用户LuaVm
	bool CreateLuaVmAfterLogin(void* cliptr, LuaVmMgr* luaVmMgrPtr, ClassDataBase* db);

	//创建LuaVm后加载Mysql的数据（mysql->redis）
	bool LuaVmLoadMysqlDataByLogin(std::string uid, LuaVmMgr* luaVmMgrPtr, lua_State* L, ClassDataBase* db);

	//登录成功后加入socketIdMap容器
	void AddIntoSockIdMap(void* cliptr, void* sockmapPtr);

	//用户下线保存数据进入Mysql（redis->mysql）
	bool SaveLuaScriptDataIntoDB(std::string uid, LuaVmMgr* luaVmMgrPtr, lua_State* L, ClassDataBase* db);

	//下线后移出socketIdMap容器
	void RemoveFromSockIdMap(void* cliptr, void* sockmapPtr, std::string uid);

	//判断是否需要重登处理(有可能之前掉线了, 导致没有正常下线, 以至于数据还残存)
	void CheckoutReLogin(std::string uid, LuaVmMgr* luaVmMgrPtr, void* sockidmapPtr, void* sockfdmapPtr, ClassServer* classServerPtr);
}

#endif