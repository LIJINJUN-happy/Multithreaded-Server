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
    std::map<std::string, std::pair<int, std::string>> LuaMoudleFilesInfo{
        
        //个人模块（加载在PersonalVm上）
        {"ACTOR",std::make_pair(LuaVmType::PERSONAL,"Actor.lua 路径")},   //用户模块
        {"BAG",std::make_pair(LuaVmType::PERSONAL,"Bag.lua 路径")},       //背包模块
        {"EMAIL",std::make_pair(LuaVmType::PERSONAL,"Email.lua 路径")},   //邮件模块

        //公共模块（加载在PublicVm上）
        {"RANK",std::make_pair(LuaVmType::PUBLIC,"Rank.lua 路径")},       //排行榜模块
        {"ROOM",std::make_pair(LuaVmType::PUBLIC,"Room.lua 路径")}        //副本/房间模块
    };
}

#endif