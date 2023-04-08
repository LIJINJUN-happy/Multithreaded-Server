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
using namespace std;

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[90]="{\"Moudle\":\"LOGIN\",\"Protocol\":\"c_registered_token_request\",\"score\":\"100\"}|";
    int str_len = 0;
    int idx = 0, read_len = 0; 
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
	{return 0;}
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.105");
    serv_addr.sin_port = htons(8888);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
      {
	 cout<<" connect erro  ";
	 return 0; 
      }
     else
	{
	int ttt=1;
		while(ttt)
		{
		string sq = "{\"Moudle\":\"LOGIN\",\"Protocol\":\"c_registered_token_request\",\"score\":\"100\"}|";
		char buf[90] = {0};
		memcpy(buf,sq.c_str(),sq.size());
	cout<< "send message ttt = "<< ttt<<endl;	
	int how = 	send(sock, buf, strlen(buf), 0);
	cout<<how<<endl;
		//usleep(1000);
		ttt = ttt -1;
		}
	usleep(1000000000);		
	}
return 0;
}
