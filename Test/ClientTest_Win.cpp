#include <cstdio>
#include <iostream>
#include <json/json.h>
#include <string>
#include <cstring>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

const int MyPort = 11967;
const char* MyIp = "103.26.77.66";
pair<SOCKET, string> Obj;
HANDLE hanpid = nullptr;
string MynName;

DWORD WINAPI GetTheMessage(LPVOID); //用来Get message 的线程函数

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP);


	SOCKADDR_IN s_i;
	s_i.sin_addr.S_un.S_addr = inet_addr(MyIp);
	s_i.sin_family = AF_INET;
	s_i.sin_port = htons(MyPort);
	if (connect(sock, (const sockaddr*)&s_i, sizeof(s_i)) >= 0)
	{
		hanpid = CreateThread(NULL, 0, GetTheMessage, (LPVOID)sock, NULL, NULL);
		printf_s("Connect Success\n");
		int ifWhile = 1;
		while (ifWhile)
		{
			cout << "选择操作:" << endl;
			int num = 0;
			cin >> num;
			Json::FastWriter w;
			Json::Value v;
			string what_want_to_send;
			switch (num)
			{
			case 1:
				what_want_to_send = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_registered_token_request\",\"EmailAddress\":\"2231173990@qq.com\"}|";
				break; //注册码请求
			case 2:
				what_want_to_send = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_registered_request\",\"Account\":\"li\",\"Password\":\"abc\",\"Code\":111}|";
				break; //注册请求
			case 3:
				what_want_to_send = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_login_request\",\"Account\":\"li\",\"Password\":\"abc\"}|";
				break; //登录请求
			case 4:
				what_want_to_send = "{\"Moudle\":\"ACTOR\",\"Protocol\":\"AddScore\",\"score\":1997}|";
				break; //添加积分请求
			case 5:
				break; //
			default:
				ifWhile = 0;
				break;
			}
			if (ifWhile == 0)
				continue;

			//string what_want_to_send = w.write(v);
			cout << "发送的数据是 :" << what_want_to_send << endl;
			send(sock, what_want_to_send.c_str(), strlen(what_want_to_send.c_str()) + sizeof(char), 0);
		}
	}
	else
	{
		printf_s("Connect Not Success\n");
		WSACleanup();
		return 0;
	}
	closesocket(sock);

	return 0;
}

DWORD WINAPI GetTheMessage(LPVOID sock)
{
	char buff[1024] = { 0 };
	string str;
	while (true)
	{
		int size = recv((SOCKET)sock, buff, sizeof(buff), 0);
		if (size >= 1)
		{
			str = buff;
			cout << "接收到服务器发来的数据:" << str << endl;
		}
	}
	return 0;
}