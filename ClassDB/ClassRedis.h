#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_
//Redis类（用于存取Redis数据）

#include "../etc/SafeMap.h"

class Redis
{
public:

	//构造函数
	Redis();
	
	//释放资源
	~Redis();

	//创建连接
	bool connect(std::string host, int port);

	//检测是否存在数据
	bool checkoutData(std::string key);

	//get请求
	std::string get(std::string key);

	//keys请求--返回数组长度（REDIS_REPLY_ARRAY）
	const int getKeysMatchingLen(std::string key);

	//获取keys指令返回的数据
	std::string getKeysMatchingResultWithIndex(int index);

	//set请求
	void set(std::string key, std::string value);

	//Release Redis Info
	void release(std::string key, std::string moudle);

	//主动释放内村
	void freeRedis();

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif