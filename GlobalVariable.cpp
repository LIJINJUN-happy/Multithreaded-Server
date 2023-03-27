#include "GlobalVariable.h"

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
    this->moudleInfo = {

        //个人模块（加载在PersonalVm上）
        {"ACTOR",std::make_pair(LuaVmType::PERSONAL,"Actor.lua 路径")},   //用户模块
        {"BAG",std::make_pair(LuaVmType::PERSONAL,"Bag.lua 路径")},       //背包模块
        {"EMAIL",std::make_pair(LuaVmType::PERSONAL,"Email.lua 路径")},   //邮件模块

        //公共模块（加载在PublicVm上）
        {"RANK",std::make_pair(LuaVmType::PUBLIC,"Rank.lua 路径")},       //排行榜模块
        {"ROOM",std::make_pair(LuaVmType::PUBLIC,"Room.lua 路径")}        //副本/房间模块
    };

    using std::fstream;
    std::fstream loader;
    loader.open("./LuaServer/MoudleFilesLoad.txt");
    assert(loader.is_open());
    std::string loadString;
    while (std::getline(loader,loadString))
    {
        std::cout << loadString << std::endl;
    }
}

std::map<std::string, std::pair<int, std::string>>* Global::LuaMoudleFilesInfo::GetMoudleInfo()
{
    return &(this->moudleInfo);
}
