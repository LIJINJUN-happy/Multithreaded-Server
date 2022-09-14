#pragma once
#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

namespace Global
{
    long GetNowTime();
}

#endif