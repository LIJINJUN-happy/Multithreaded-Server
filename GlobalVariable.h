#pragma once
#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

namespace Global
{
    long GetNowTime(); //获取当前时间戳

    void ServerQuit(int signum); /*(ctrl + \ )*/
    void SignalReady();          //信号处理准备工作
}

#endif