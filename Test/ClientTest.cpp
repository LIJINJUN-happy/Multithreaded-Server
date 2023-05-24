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

const int total = 20;

int main(int argc, char* argv[])
{
	int sock[total] = {};
	struct sockaddr_in serv_addr[total];
	for (int i = 0; i < total; i++)
	{
		sock[i] = socket(PF_INET, SOCK_STREAM, 0);
		if (sock[i] == -1)
		{
			return 0;
		}
		memset(&serv_addr[i], 0, sizeof(serv_addr[i]));
		serv_addr[i].sin_family = AF_INET;
		serv_addr[i].sin_addr.s_addr = inet_addr(Config::addrString.c_str());
		serv_addr[i].sin_port = htons(Config::listenPort);
		if (connect(sock[i], (struct sockaddr*)&(serv_addr[i]), sizeof(serv_addr[i])) == -1)
		{
			cout << " connect erro  ";
			return 0;
		}
	}
	string sq = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_login_request\",\"Account\":\"li\",\"Password\":\"abc\"}|";
	char buf[256] = { 0 };
	memcpy(buf, sq.c_str(), sq.size());
	for (int i = 0; i < total; i++)
	{
		int how = send(sock[i], buf, strlen(buf), 0);
		cout << "send Size = " << how << endl;
	}

	char c = getchar();
	return 0;
}
