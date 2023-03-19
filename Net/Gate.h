#ifndef _GATE_H_
#define _GATE_H_

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "./ClassMsgPackage.h"

//转换为char*然后send
void TransformationAndSend(std::string msg);

//接受recv的数据data然后分解里面有多少协议
std::vector<std::string> GetAndTransformation(std::string msg);

#endif