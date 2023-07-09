#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_

#include "../Logger/ClassLogger/ClassLogger.h"
ClassLogger LOG;

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

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif