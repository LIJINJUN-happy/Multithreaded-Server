#ifndef _GATE_H_
#define _GATE_H_

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "./ClassMsgPackage.h"

//预声明
class Client;
class ClassTcpNet;

//转换为char*然后send（弃用）
void TransformationAndSend(std::string msg);

//接受recv的数据data然后分解里面有多少协议（弃用）
std::vector<std::string> GetAndTransformation(std::string msg);

//注册码请求处理
void GetRegisteredToken(int fd, void* netptr);

//注册请求处理
bool Registered(int fd, void* netptr, std::string account, std::string pw, std::string token);

//登录请求处理
bool Login(int fd, void* netptr, std::string account, std::string pw);

#endif