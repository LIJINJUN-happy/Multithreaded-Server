#include "Gate.h"

void packMsg(string msg)
{
    //信息长度
    int msgSize = msg.size();
    cout << "打包的信息字符串长度为：" << msgSize << endl;
    if (msgSize <= 0)
    {
        return;
    }

    //信息长度值的长度
    string sizeLenString = to_string(msgSize);
    int sizeLen = sizeLenString.size();
    cout << "大小值" << msgSize << "的长度为" << sizeLen << endl;

    //信息长度值的长度的位数（有几位数）
    string strMgsTotalNum = to_string(sizeLen);

    //位数值 + 长度值 + 信息 + 结尾符'\0'
    string finalStr = strMgsTotalNum + sizeLenString + msg;
    const char *buf = finalStr.c_str();
    const int totalSize = (const int)strlen(buf);

    //打印一下buf内容
    for (size_t i = 0; i < totalSize; i++)
    {
        cout << buf[i];
    }
    cout << endl;

    return;
}