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
#include <sys/syscall.h>
#include <pthread.h>
#include <memory>
#include <algorithm>
#include <atomic>
using namespace std;

const int total = 500;
std::atomic<int> login_total = 0;

void* Looping(void* sock)
{

	char dataBuff[Config::maxReadDataSize] = "";
	int login = 0;
	while (1)
	{
		memset(dataBuff, 0, 0);
		int socket = *((int*)sock);
		int resRead = recv(socket, dataBuff, sizeof(dataBuff), MSG_DONTWAIT);
		if (resRead <= 0)
		{
			continue;
		}
		string getString(dataBuff, 0, resRead);
		cout << "getString = " << getString << std::endl;
		login_total.fetch_add(1);

		if (login == 0)
		{
			cout << "load total = " << login_total.load() << std::endl;
			login = login + 1;
		}
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int sock[total] = {};
	struct sockaddr_in serv_addr[total];
	pthread_t timeTid[total];
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
		else
		{
			timeTid[i] = 0;
			int resTimerCreate = pthread_create(&(timeTid[i]), NULL, Looping, &(sock[i]));
		}
	}


	string sq = "{\"Moudle\":\"GATE\",\"Protocol\":\"c_login_request\",\"Account\":\"li\",\"Password\":\"abc\"}|";
	char buf[256] = { 0 };
	memcpy(buf, sq.c_str(), sq.size());
	for (int i = 0; i < total; i++)
	{
		int how = send(sock[i], buf, strlen(buf), 0);
		//printf("[i] == %d  send Size = %d \n",i,how);
	}

	while (1)
	{
		if (total == login_total)
		{
			std::cout << "µÇÂ¼Íê³É£¡" << std::endl;
			break;
		}
		else
			continue;
	}
	char c = getchar();

	sq = "{\"Moudle\":\"ACTOR\",\"Protocol\":\"AddScore\",\"score\":1997}|";
	memcpy(buf, sq.c_str(), sq.size());
	for (int i = 0; i < total; i++)
	{
		int how = send(sock[i], buf, strlen(buf), 0);
		//printf("[i] == %d  send Size = %d \n", i, how);
	}
	return 0;
}

//g++ ./ClientTest.cpp  -o ClientTest.out -lpthread   -std=c++11