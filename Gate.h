#ifndef _GATE_H_
#define _GATE_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>

using namespace std;

//转换为char*然后send
void TransformationAndSend(string msg);

//接受recv的数据data然后分解里面有多少协议
vector<string> GetAndTransformation(string msg);

#endif