#include "ClassLogger.h"

using std::cout;
using std::endl;
using std::string;

ClassLogger::ClassLogger()
{
	this->writer.flush();
}

ClassLogger::~ClassLogger()
{
	writer.close();
}

std::ofstream& ClassLogger::Log(std::string type)
{
	
}

std::ofstream& ClassLogger::Log()
{
	this->writer << "[ " << pthread_self() << " ] : ";
	return this->writer;
}

void ClassLogger::Init()
{
	std::string path = Config::LoadCodePathString;
	path += "Logger/LogInfo/main_Logger.txt";;
	this->writer.open(path.c_str(),std::fstream::app|std::fstream::out);
	if (!writer.is_open())
	{
		cout << "Logger Files Open Fail" << endl;
		assert(false);
	}
	else
	{
		this->Log() << "Logger Init Success " << std::endl;
		//writer.flush;
	}
}
