#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_
//Redis类（用于存取Redis数据）

#include "../Logger/ClassLogger/ClassLogger.h"

class Redis
{
public:

	//构造函数
	Redis();
	
	//释放资源
	~Redis();

	//创建连接
	bool connect(std::string host, int port);

	//get请求
	std::string get(std::string key);

	//set请求
	void set(std::string key, std::string value);

	//Release Redis Info
	void release(std::string key, std::string moudle) {};

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif