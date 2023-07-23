#include "ClassRedis.h"
ClassLogger LOG;

Redis::Redis()
{
	this->_connect = NULL;
	this->_reply = NULL;
}

Redis::~Redis()
{
	this->_connect = NULL;
	this->_reply = NULL;
}

bool Redis::connect(std::string host, int port)
{
	this->_connect = redisConnect(host.c_str(), port);
	if (this->_connect != NULL && this->_connect->err)
	{
		LOG.Log() << "连接Redis Error = " << this->_connect->errstr << std::endl;
		return false;
	}
	return true;
}

bool Redis::checkoutData(std::string key)
{
	this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
	if (this->_reply->type == REDIS_REPLY_NIL)
	{
		return false;
	}
	return true;
}

std::string Redis::get(std::string key)
{
	this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
	std::string str = this->_reply->str;
	freeReplyObject(this->_reply);//释放redisCommand执行后返回的的redisReply所占用的内存
	return str;
}

void Redis::set(std::string key, std::string value)
{
	redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
}
