#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_

#include "../Logger/ClassLogger/ClassLogger.h"
ClassLogger LOG;

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

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif