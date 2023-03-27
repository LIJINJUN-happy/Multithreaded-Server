#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <atomic>

namespace Config
{
	//线程配置
	const int pollingPthreadNum = 3; //轮询任务列表线程数量

	//监听以及epoll配置
	const int maxConnect = 256;						//最大同时连接列表值（listen 第二参数）
	const int listenPort = 8888;					//监听端口号
	const std::string addrString = "192.168.0.107";	//地址
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

	//lua模块加载文件路径
	const std::string luaMoudleString = "/home/lijinjun/公共/debug_env/LuaServer/MoudleFilesLoad.txt";
}

#endif