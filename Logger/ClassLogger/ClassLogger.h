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

	//Lua ��־���,typeΪluaģ��
	std::ofstream & Log(std::string type);
	
	//��־�����c++�У�
	std::ofstream& Log();

	//���������c++�У�
	std::ofstream& Error();

	void Init();
};

#endif