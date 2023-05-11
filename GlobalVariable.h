#pragma once
#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#include "Logger/ClassLogger/ClassLogger.h"

//logger全局声明
class ClassLogger;
extern ClassLogger LOG;

namespace Global
{
    long GetNowTime(); //获取当前时间戳

    struct tm* GetNowTimeWithFormat();

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
        bool addOneLuaMoudle(std::string& loadString,int type);
        std::map<std::string, std::pair<int, std::string>>* GetMoudleInfo();
        int judgeMoudleType(std::string);
    };

    //C++的协议发送包结构
    struct MakeSendPackage
    {
    public:
        Json::Value val;

    public:
        MakeSendPackage(std::string moudle, std::string proto);
        ~MakeSendPackage();
    
    public:
        int Flush(int socket); //发送协议内容
        void SetVal(std::string tag, int arg);
        void SetVal(std::string tag, std::string arg);
        void SetVal(std::string tag, bool arg);
        void SetVal(std::string tag, double arg);
        void SetVal(std::string tag, float arg);
    };

    //Lua发送协议
    static int LuaSendMessage(lua_State* L);
}

#endif