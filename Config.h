#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

namespace Config
{
	const int maxConnect = 1;						//最大同时连接列表值（listen 第二参数）
	const int listenPort = 8888;					//监听端口号
	const std::string addrString = "192.168.0.104"; //地址
	const int pollingPthreadNum = 6;				//轮询任务列表线程数量
	const int maxSocketfd = 200;					//最大socket对象监听上限值
	const int maxEpollEvent = 1024;					//最大epoll监视事件返回个数
	const int maxReadDataSize = 1024;				//读取sockfd数据大小上限值
}

#endif