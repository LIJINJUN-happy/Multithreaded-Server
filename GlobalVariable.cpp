#include "GlobalVariable.h"

//获取当前时间戳
long Global::GetNowTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long resTime = tv.tv_sec;
    return resTime;
}