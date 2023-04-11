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
	std::ofstream & Log(std::string type);//Lua ��־���,typeΪluaģ��
	std::ofstream& Log();				  //��־�����c++�У�
	std::ofstream& Error();				  //���������c++�У�
	void Init();
};

#endif