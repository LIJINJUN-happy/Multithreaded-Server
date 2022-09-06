#pragma once
#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

typedef struct Task
    {
        vector<string> TaskList;
        pthread_mutex_t lock;
    } Task;

extern Task task;

#endif