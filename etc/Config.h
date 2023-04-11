#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <atomic>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <utility>
#include <map>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <assert.h>
#include <sys/select.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <initializer_list>

namespace Config
{
	//服务器信息
	const std::string servName = "LIJINJUN_SER";//服务器名称
	const int servID = 1997;					//服务器ID
	const long serOpenTime = 0;					//开服时间

	//线程配置
	const int pollingPthreadNum = 3; //轮询任务列表线程数量

	//监听以及epoll配置
	const int maxConnect = 256;						//最大同时连接列表值（listen 第二参数）
	const int listenPort = 8888;					//监听端口号
	const std::string addrString = "192.168.0.105";	//地址
	const int maxSocketfd = 1024;					//最大socket对象监听上限值
	const int maxEpollEvent = 1024;					//最大epoll监视事件返回个数
	const int maxReadDataSize = 256;				//读取sockfd数据大小上限值

	//数据库配置
	const std::string host = "localhost";	  //主机地址
	const std::string user = "root";		  //用户，例如：root
	const std::string passwd = "lijinjun123"; //数据库的密码
	const std::string db = "";				  //要连接的数据库，例如：student
	const unsigned int port = 3306;			  //端口
	const unsigned long client_flag = 0;	  //一般为0
	// const char *unix_socket = NULL;		  //一般为NULL

	//监视检测配置
	const int HeartbeatIntervalTime = 100; //允许距离上次心跳最大时间
	const int CheckoutIntervalTime = 120;  //监视线程多久检测一次心跳以及线程状态

	//定时器线程配置
	const int timerIntervalTime = 1; //计时器精度秒数

	//项目代码目录（部署时必须修改对应正确项目路径）
	const std::string LoadCodePathString = "/home/lijinjun/公共/debug_env/";

	//验证码信息
	const int registerCodeSize = 6;				//验证码长度
	const long registerCodeOutTimeInterval = 60;//验证码有效秒数

}

#endif