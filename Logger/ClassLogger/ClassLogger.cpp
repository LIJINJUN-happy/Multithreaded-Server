#include "ClassLogger.h"

using std::cout;
using std::endl;
using std::string;

ClassLogger::ClassLogger()
{
	this->logWriter.flush();
	this->ErrWriter.flush();
}

ClassLogger::~ClassLogger()
{
	this->logWriter.close();
	this->ErrWriter.close();
}

std::ofstream& ClassLogger::Log(std::string type)
{
	
}

std::ofstream& ClassLogger::Log()
{
	time_t timer = time(NULL);
	struct tm* localtm = localtime(&timer);
	this->logWriter << "[" << pthread_self() << "-"
		<< localtm->tm_year + 1990 << "/"
		<< localtm->tm_mon + 1 << "/"
		<< localtm->tm_mday << " "
		<< localtm->tm_hour << ":"
		<< localtm->tm_min << ":"
		<< localtm->tm_sec
		<< "] : ";
	return this->logWriter;
}

std::ofstream& ClassLogger::Error()
{
	time_t timer = time(NULL);
	struct tm* localtm = localtime(&timer);
	this->ErrWriter << "[" << pthread_self() << "-"
		<< localtm->tm_year + 1990 << "/"
		<< localtm->tm_mon + 1 << "/"
		<< localtm->tm_mday << " "
		<< localtm->tm_hour << ":"
		<< localtm->tm_min << ":"
		<< localtm->tm_sec
		<< "] : ";
	return this->ErrWriter;
}

void ClassLogger::Init()
{
	std::string path = Config::LoadCodePathString;
	path += "Logger/LogInfo/main_Erro.txt";
	this->ErrWriter.open(path.c_str(), std::fstream::app | std::fstream::out);
	if (!this->ErrWriter.is_open())
	{
		assert(false);
		return;
	}
	path.clear();

	path = Config::LoadCodePathString;
	path += "Logger/LogInfo/main_Logger.txt";
	this->logWriter.open(path.c_str(),std::fstream::app|std::fstream::out);
	if (!logWriter.is_open())
	{
		this->Error() << "\033[31mLogger Files Open Fail\033[0m" << endl;
		assert(false);
	}
	else
	{
		this->Log() << "\033[35mLogger Init Success! \033[0m" << std::endl << std::endl;
		//writer.flush;
	}
	return;
}
