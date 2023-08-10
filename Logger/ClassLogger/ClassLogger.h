#ifndef _CLASSLOGGER_H_
#define _CLASSLOGGER_H_

#include "../../etc/Config.h"
#include "../../ClassDB/DBCommand.h"

class ClassLogger
{
private:
	std::ofstream logWriter;
	std::ofstream ErrWriter;
public:
	ClassLogger();
	~ClassLogger();

public:

	//Lua 日志输出,type为lua模块
	std::ofstream & Log(std::string type);
	
	//日志输出（c++中）
	std::ofstream& Log();

	//错误输出（c++中）
	std::ofstream& Error();

	void Init();
};

#endif