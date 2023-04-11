#ifndef _CLASSLOGGER_H_
#define _CLASSLOGGER_H_

#include "../../etc/Config.h"

class ClassLogger
{
private:
	std::ofstream writer;

public:
	ClassLogger();
	~ClassLogger();

public:
	std::ofstream & Log(std::string type);//Lua 日志输出,type为lua模块
	std::ofstream& Log();				  //日志输出（c++中）
	void Init();
};

#endif