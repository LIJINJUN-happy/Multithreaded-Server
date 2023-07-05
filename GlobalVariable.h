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

    enum ServerStatus
    {
        Running = 1,//正常运作
        Fixing,     //维护（不接受连接以及任何客户端数据）
        Codeing,    //调试（只接受指定的UID连接以及协议数据）
    };

    enum ServerConnectCondition
    {
        Fluency = 1,//流畅
        Normal,     //正常
        Hotly,      //火热
        Fully,      //拥堵（爆满）
    };

    std::string BreakDownByString(std::string &str, std::string posString);
}

#endif