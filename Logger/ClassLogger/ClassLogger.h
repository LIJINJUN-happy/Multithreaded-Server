#ifndef _CLASSLOGGER_H_
#define _CLASSLOGGER_H_

#include "../../etc/Config.h"

class ClassLogger
{
private:
	std::ofstream logWriter;
	std::ofstream ErrWriter;
public:
	ClassLogger();
	~ClassLogger();

public:
	std::ofstream & Log(std::string type);//Lua 日志输出,type为lua模块
	std::ofstream& Log();				  //日志输出（c++中）
	std::ofstream& Error();				  //错误输出（c++中）
	void Init();
};

#endif