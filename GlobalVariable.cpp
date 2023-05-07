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

tm* Global::GetNowTimeWithFormat()
{
    time_t timer = time(NULL);
    struct tm* localtm = localtime(&timer);
    return localtm;
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
            //LOG.Log() << "加载完之后 moudleInfo.size = " << moudleInfo.size() << std::endl;
        }
    }
    loader.close();
}

bool Global::LuaMoudleFilesInfo::addOneLuaMoudle(std::string& loadString,int type)
{
    int commaCharIndex = loadString.find(",");
    if (commaCharIndex == std::string::npos)
    {
        LOG.Log() << "SomeThing Wrong With Function addOneLuaMoudle : Cant't Find Char ',' In loadString" << std::endl;
        LOG.Log() << "loadString" << loadString << "type" << type << std::endl;
        return false;
    }
    std::string moudleName(loadString, 0, commaCharIndex);
    std::string loadPathString(loadString, commaCharIndex + 1, loadString.npos);
    loadPathString.insert(0, Config::LoadCodePathString);
    if (access(loadPathString.c_str(), F_OK) != 0)
    {
        LOG.Log() << "文件不存在："<< loadPathString << std::endl;
        assert(false);
    }
    //LOG.Log() << "moudleName = " << moudleName << "   loadPathString = " << loadPathString << std::endl;
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

Global::MakeSendPackage::MakeSendPackage(std::string moudle, std::string proto)
{
    val["Moudle"] = Json::Value(moudle);
    val["Protocol"] = Json::Value(proto);
}

Global::MakeSendPackage::~MakeSendPackage()
{
}

int Global::MakeSendPackage::Flush(int socket)
{
    Json::FastWriter writer;
    std::string jsonStr = writer.write(val);
    jsonStr.push_back('|');
    char buf[256] = { 0 };
    memcpy(buf, jsonStr.c_str(), jsonStr.size());
    int sendSize = send(socket, buf, strlen(buf), 0);
    return sendSize;
}

void Global::MakeSendPackage::SetVal(std::string tag, int arg)
{
    val[tag.c_str()] = Json::Value(arg);
}

void Global::MakeSendPackage::SetVal(std::string tag, std::string arg)
{
    val[tag.c_str()] = Json::Value(arg);
}

void Global::MakeSendPackage::SetVal(std::string tag, bool arg)
{
    val[tag.c_str()] = Json::Value(arg);
}

void Global::MakeSendPackage::SetVal(std::string tag, double arg)
{
    val[tag.c_str()] = Json::Value(arg);
}

void Global::MakeSendPackage::SetVal(std::string tag, float arg)
{
    val[tag.c_str()] = Json::Value(arg);
}


