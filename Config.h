#include <string>

namespace Config{
	const int maxConnect = 1;						//最大连接数量
	const int listenPort = 8888;					//监听端口号
	const std::string addrString = "192.168.0.104";	//地址
	const int pollingPthreadNum = 6;				//轮询任务列表线程数量
}