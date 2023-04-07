#include "Gate.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

//转换为char*然后send
void TransformationAndSend(string msg)
{
    //信息长度
    int msgSize = msg.size();
    // cout << "打包的信息字符串长度为：" << msgSize << endl;
    if (msgSize <= 0)
    {
        return;
    }

    //信息长度值的长度
    string sizeLenString = std::to_string(msgSize);
    int sizeLen = sizeLenString.size();
    // cout << "大小值" << msgSize << "的长度为" << sizeLen << endl;

    //信息长度值的长度的位数（有几位数）
    string strMgsTotalNum = std::to_string(sizeLen);

    //位数值 + 长度值 + 信息
    string finalStr = strMgsTotalNum + sizeLenString + msg;

    //转换为char*类型，作为缓存容器变量
    const char *buf = finalStr.c_str();
    const int totalSize = (const int)strlen(buf);

    /*打印一下将要发送的buf内容
    for (size_t i = 0; i < totalSize; i++)
    {
        cout << buf[i];
    }
    cout << endl;
    */
    return;
}

//接受recv的数据data然后分解里面有多少协议
vector<string> GetAndTransformation(string msg)
{
    // cout << "获取到了整体msg:  " << msg << "  总长度为：" << msg.size() << endl;
    vector<string> msgTable;
    for (;;)
    {
        if (msg.size() == 0)
        {
            break;
        }
        int deleteTotal = 1;

        //获取位数
        string firstSet(msg, 0, 1);
        int strMgsTotalNum = std::stoi(firstSet);
        deleteTotal += strMgsTotalNum;
        // cout << "获取字节的位数：" << strMgsTotalNum << endl;

        //获取信息的长度
        string sizeLenString(msg, 1, strMgsTotalNum);
        int sizeLen = std::stoi(sizeLenString);
        deleteTotal += sizeLen;
        // cout << "信息长度为：" << sizeLen << endl;

        //获取信息并存入msgtable
        string realString(msg, strMgsTotalNum + 1, sizeLen);
        msgTable.push_back(realString);

        //更新msg
        msg.erase(msg.begin(), msg.begin() + deleteTotal);
    }
    return msgTable;
}

//验证码请求
void GetRegisteredToken(void* cliptr)
{
    //生产随机的数字组合
    int code = 0;
    long nTime = Global::GetNowTime();

    //设置验证码信息
    ((Client*)cliptr)->SetRegisterCode(code);
    ((Client*)cliptr)->SetRegisterCodeTime(nTime);
    return ;
}

//注册请求
bool Registered(void* cliptr, std::string account, std::string pw, std::string code)
{
    bool resRegister = ((Client*)cliptr)->JudgeRegisterCode(code);
    if (resRegister == false)
    {
        return false;
    }
    /*
    注册成功则保存信息,用来验证登录
    */
    return true;
}

bool Login(int fd, void* netptr, std::string account, std::string pw)
{
    return false;
}
