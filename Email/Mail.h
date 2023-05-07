#ifndef _MAIL_H_
#define _MAIL_H_

#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../GlobalVariable.h"

struct Base64Date6
{
	unsigned int d4 : 6;
	unsigned int d3 : 6;
	unsigned int d2 : 6;
	unsigned int d1 : 6;
};

char ConvertToBase64(char uc);

void EncodeBase64(char* dbuf, char* buf128, int len);

class Socket_RAII {
public:
	Socket_RAII(int sockfd)
	{
		m_sockfd = sockfd;
	}
	~Socket_RAII()
	{
		close(m_sockfd);
	}
private:
	int m_sockfd;
};

int SendEmail(
	const char* smtp,//�����������ַ����smtp.exmail.qq.com
	unsigned short port,//�����������Ʒ��25
	const char* account,//���ͷ��������ʺ�
	const char* password,//���ͷ��������������Ȩ��
	const char* email,//Ŀ������
	const char* body,//�ʼ����ļ����ݣ��ַ���
	const char* from_name,//�ʼ����ͷ�������
	const char* to_name,//�ʼ����շ�������
	const char* subject);//�ʼ�����

/*
void SendRegisterCode(const char* smtp,
	unsigned short port,
	const char* account,
	const char* password,
	const char* email,
	const char* body,
	const char* from_name,
	const char* to_name,
	const char* subject);
*/
#endif