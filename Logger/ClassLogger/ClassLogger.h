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
	std::ofstream & Log(std::string type);//Lua ��־���,typeΪluaģ��
	std::ofstream& Log();				  //��־�����c++�У�
	void Init();
};

#endif