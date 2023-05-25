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
	const char* smtp,//邮箱服务器地址：如smtp.exmail.qq.com
	unsigned short port,//邮箱服务器商品：25
	const char* account,//发送方的邮箱帐号
	const char* password,//发送方的邮箱密码或授权码
	const char* email,//目标邮箱
	const char* body,//邮件正文件内容，字符串
	const char* from_name,//邮件发送方的名称
	const char* to_name,//邮件接收方的名称
	const char* subject);//邮件主题

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