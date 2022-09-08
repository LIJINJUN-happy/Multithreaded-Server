#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
using namespace std;
	
void packMsg(string s);
void unpackMsg(char msg);
int main()
{
	int m = 0;
	char buf[1024]="1024";
	for(int i = 0 ;;i++)
	{
		if(buf[i]== '\0')
		{
			break;
		}
		m += 1;
	}
	const int total = m;
	cout << "the buf true size if = " << m << endl;
	string str = "str";
	cout << "sizeof(str) ="<< sizeof(str)<< endl;
	cout << "sizeof(string) ="<< sizeof(string)<< endl;
	const int len = str.size();
	char newone[len];
	str.copy(newone,len,0);
	cout << "用函数copy转换后 sizeof =" << sizeof(newone) <<endl;
	for(int i=0;i<len;i++)
	{
		cout<<"第"<<i<<"个字符="<<newone[i]<<endl;
	}
	cout << "sizeof(4)= " << sizeof(4) << endl;
	cout << "string'111'" << sizeof("111") << endl;
	cout << "sizeof(buf)= " << sizeof(buf)<< endl;
	char truebuf[total];
	for(int i=0;i<total;i++)
	{
		truebuf[i] = buf[i];
	}
	cout << "sizeof(truebuf)= " << sizeof(truebuf)<< endl ;
	printf("after copy the truebuf = %s\n",truebuf);		
	return 0;
}
void packMsg(string s)
{
	
}
void unpackMsg(char msg)
{
}
