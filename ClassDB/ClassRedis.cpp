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
	//LOG.Log() << "Redis CheckoutData : " << key << "Type = " << _reply->type << std::endl;
	if (this->_reply->type == REDIS_REPLY_NIL)
	{
		freeReplyObject(this->_reply);
		return false;
	}
	freeReplyObject(this->_reply);
	return true;
}

std::string Redis::get(std::string key)
{
	this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());

	if ((this->_reply->type == REDIS_REPLY_NIL) || (this->_reply->type == REDIS_REPLY_ERROR))
	{
		LOG.Log() << "Redis::get : " << key << ",   ResultType = " << _reply->type << std::endl;
		freeReplyObject(this->_reply);
		return "";
	}

	if (this->_reply->type != REDIS_REPLY_STRING)
	{
		LOG.Log() << "Redis::get : " << key << ",   ResultType = " << _reply->type << std::endl;
		freeReplyObject(this->_reply);
		return "";
	}

	std::string str = this->_reply->str;
	freeReplyObject(this->_reply);//释放redisCommand执行后返回的的redisReply所占用的内存
	return str;
}

const int Redis::getKeysMatchingLen(std::string key)
{
	this->_reply = (redisReply*)redisCommand(this->_connect, "KEYS %s", key.c_str());
	int len = this->_reply->elements;
	return len;
}

std::string Redis::getKeysMatchingResultWithIndex(int index)
{
	return std::string(_reply->element[index]->str);
}

void Redis::set(std::string key, std::string value)
{
	redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
}

void Redis::release(std::string key, std::string moudle)
{
	key += "_";
	key += moudle;
	redisCommand(this->_connect, "DEL %s", key.c_str());
}

void Redis::freeRedis()
{
	freeReplyObject(this->_reply);//释放redisCommand执行后返回的的redisReply所占用的内存
}
