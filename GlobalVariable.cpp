#include "GlobalVariable.h"

ClassLogger LOG;

//获取当前时间戳
long Global::GetNowTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long resTime = tv.tv_sec;
    return resTime;
}

/*(ctrl + \ )*/
void Global::ServerQuit(int signum)
{
    exit(0);
}

//信号处理准备工作
void Global::SignalReady()
{
    // signal(SIGINT, process_exit);
    // signal(SIGFPE, process_exit);
    // signal(SIGILL, process_exit);
    // signal(SIGABRT, process_exit);
    // signal(SIGSEGV, process_exit);
    // signal(SIGTERM, process_exit);
    signal(SIGQUIT, ServerQuit);
}

Global::LuaMoudleFilesInfo::LuaMoudleFilesInfo()
{
    this->moudleInfo.clear();
}

void Global::LuaMoudleFilesInfo::LoadLuaMoudleFiles()
{
    this->moudleInfo.clear();
    using std::fstream;
    std::fstream loader;
    std::string loadPath = Config::LoadCodePathString + "LuaServer/MoudleFilesLoad.txt";
    loader.open(loadPath.c_str());
    assert(loader.is_open());
    std::string loadString;
    bool isPersonalFile = true;
    while (std::getline(loader,loadString))
    {
        if (loadString.find("Public Moudle") != std::string::npos)
        {
            isPersonalFile = false;
        }

        if (loadString.find("{") != std::string::npos)
        {
            int index = loadString.find("}");
            loadString.assign(loadString, 1, index - 1);
            int type = isPersonalFile == true ? Global::PERSONAL : Global::PUBLIC;
            bool resultLoad = this->addOneLuaMoudle(loadString, type);
            assert(resultLoad);
            //std::cout << "加载完之后 moudleInfo.size = " << moudleInfo.size() << std::endl;
        }
    }
    loader.close();
}

bool Global::LuaMoudleFilesInfo::addOneLuaMoudle(std::string& loadString,int type)
{
    int commaCharIndex = loadString.find(",");
    if (commaCharIndex == std::string::npos)
    {
        std::cout << "SomeThing Wrong With Function addOneLuaMoudle : Cant't Find Char ',' In loadString" << std::endl;
        std::cout << "loadString" << loadString << "type" << type << std::endl;
        return false;
    }
    std::string moudleName(loadString, 0, commaCharIndex);
    std::string loadPathString(loadString, commaCharIndex + 1, loadString.npos);
    loadPathString.insert(0, Config::LoadCodePathString);
    if (access(loadPathString.c_str(), F_OK) != 0)
    {
        std::cout<< "文件不存在："<< loadPathString << std::endl;
        assert(false);
    }
    //std::cout << "moudleName = " << moudleName << "   loadPathString = " << loadPathString << std::endl;
    this->moudleInfo.insert(std::make_pair(moudleName,std::make_pair(type,loadPathString)));
    loadString.clear();
    return true;
}

std::map<std::string, std::pair<int, std::string>>* Global::LuaMoudleFilesInfo::GetMoudleInfo()
{
    return &(this->moudleInfo);
}

int Global::LuaMoudleFilesInfo::judgeMoudleType(std::string moudle)
{
    auto p = this->moudleInfo.find(moudle);
    if (p->second.first == Global::PERSONAL)
    {
        return Global::PERSONAL;
    }
    return Global::PUBLIC;
}