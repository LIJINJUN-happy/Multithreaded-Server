#pragma once
#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <utility>
#include <map>
#include <jsoncpp/json/json.h >

namespace Global
{
    long GetNowTime(); //获取当前时间戳

    //信号处理
    void ServerQuit(int signum); /*(ctrl + \ )*/
    void SignalReady();          //信号处理准备工作

    //Lua虚拟机类型
    enum LuaVmType
    {
        PERSONAL = 1,   //个人类型的LuaVm
        PUBLIC          //公共
    };

    //Lua模块信息
    struct LuaMoudleFilesInfo
    {
    public:
        LuaMoudleFilesInfo();
        ~LuaMoudleFilesInfo() {};
    public:
        std::map<std::string, std::pair<int, std::string>> moudleInfo;
    public:
        void LoadLuaMoudleFiles();
        std::map<std::string, std::pair<int, std::string>>* GetMoudleInfo();
    };
}

#endif