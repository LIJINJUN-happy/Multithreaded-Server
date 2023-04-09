#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <cstring>
#include "../etc/Config.h"
using namespace std;

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
	{
		return 0;
	}
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(Config::addrString.c_str());
    serv_addr.sin_port = htons(Config::listenPort);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
		cout<<" connect erro  ";
		return 0; 
    }
     else
	 {
		while(true)
		{
			char c = getchar();
			string sq = "";

			if (c == 'q')//登录请求
				sq = "{\"Moudle\":\"LOGIN\",\"Protocol\":\"c_login_request\",\"Account\":\"a\",\"Password\":\"b\"}|";
			if (c == 'w')//注册请求
			{
				int num = 0;
				cout << "please input code :" << endl;
				cin >> num;
				sq = "{\"Moudle\":\"LOGIN\",\"Protocol\":\"c_registered_request\",\"Account\":\"a\",\"Password\":\"b\",\"Code\":\"" + std::to_string(num) + "\"}|";
			}
			if (c == 'e')//注册码请求
				sq = "{\"Moudle\":\"LOGIN\",\"Protocol\":\"c_registered_token_request\",\"EmailAddress\":\"2231173990@qq.com\"}|";
			if (c == 'r')//调用LuaVm的添加积分请求
				sq = "{\"Moudle\":\"ACTOR\",\"Protocol\":\"AddScore\",\"score\":\"100\"}|";
			if (c == 't')//quit
				break;	

			char buf[256] = {0};
			memcpy(buf,sq.c_str(),sq.size());	
			int how = send(sock, buf, strlen(buf), 0);
			cout << "send Size = " << how << endl;
			cout << "Send Info = " << sq << endl;
		}
	 //usleep(1000000000);		
	 }
	 return 0;
}
