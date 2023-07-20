#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_
//Redis�ࣨ���ڴ�ȡRedis���ݣ�

#include "../Logger/ClassLogger/ClassLogger.h"

class Redis
{
public:

	//���캯��
	Redis();
	
	//�ͷ���Դ
	~Redis();

	//��������
	bool connect(std::string host, int port);

	//get����
	std::string get(std::string key);

	//set����
	void set(std::string key, std::string value);

	//Release Redis Info
	void release(std::string key, std::string moudle) {};

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif