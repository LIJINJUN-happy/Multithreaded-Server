#ifndef _CLASSREDIS_H_
#define _CLASSREDIS_H_
//Redis�ࣨ���ڴ�ȡRedis���ݣ�

#include "../etc/SafeMap.h"

class Redis
{
public:

	//���캯��
	Redis();
	
	//�ͷ���Դ
	~Redis();

	//��������
	bool connect(std::string host, int port);

	//����Ƿ��������
	bool checkoutData(std::string key);

	//get����
	std::string get(std::string key);

	//keys����--�������鳤�ȣ�REDIS_REPLY_ARRAY��
	const int getKeysMatchingLen(std::string key);

	//��ȡkeysָ��ص�����
	std::string getKeysMatchingResultWithIndex(int index);

	//set����
	void set(std::string key, std::string value);

	//Release Redis Info
	void release(std::string key, std::string moudle);

	//�����ͷ��ڴ�
	void freeRedis();

private:
	redisContext* _connect;
	redisReply* _reply;

};


#endif