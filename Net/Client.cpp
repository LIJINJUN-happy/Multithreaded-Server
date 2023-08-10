#include "Client.h"

using std::cout;
using std::endl;
using std::string;
using std::atomic_int;

//默认构造函数
Client::Client()
{
    this->fd = -1;
    this->uid = "";
    this->ipAddr = "";
    this->lastHeartBeatTime = Global::GetNowTime();
    this->pMyself = this;
    this->messageResidue = "";
    this->workPthreadIndex = -1;
    this->workPthreadSameClientTaskNum = 0;
    this->registerCode = 0;
    this->registerCodeTime = 0;
    this->emailAddress = "";
}

//析构函数
Client::~Client()
{
    //LOG.Log() << "调用了析构函数" << std::endl;
}

//自定义构造函数
Client::Client(int clientFd, string clientUid, string clientIp)
{
    this->fd = clientFd;
    this->uid = clientUid;
    this->ipAddr = clientIp;
    this->lastHeartBeatTime = Global::GetNowTime();
    this->pMyself = this;
    this->messageResidue = "";
    this->workPthreadIndex = -1;//默认一开始没有线程执行该用户的请求
    this->workPthreadSameClientTaskNum = 0;
    //workPthreadIndex.store(-1);
    //workPthreadSameClientTaskNum.store(0);
    this->registerCode = 0;
    this->registerCodeTime = 0;
    this->emailAddress = "";
}

//更新心跳时间
void Client::UpdateHeartBeatTime()
{
    long nowTime = Global::GetNowTime();
    this->lastHeartBeatTime = nowTime;
}

int Client::GetClientFd()
{
    return this->fd;
}

void Client::SetClientUid(std::string id)
{
    this->uid = id;
    return;
}

//查看距离上次的心跳时间差是否超出范围
bool Client::CheckoutIfOnLine()
{
    long nowTime = Global::GetNowTime();
    int diff = abs(nowTime - this->lastHeartBeatTime);

    //假如间隔时间大于允许的最大心跳间隔时间
    if (diff > Config::heartbeatIntervalTime)
    {
        LOG.Log() << "距离上次的心跳间隔为：" << diff << "  ,大于规定的" << Config::heartbeatIntervalTime << endl;
        return false;
    }

    return true;
}

void Client::UpdateMessageResidue(string newString)
{
    this->messageResidue = newString;
    return;
}

string Client::GetMessageResidue()
{
    return this->messageResidue;
}

void Client::UpdateWorkPthreadIndex(int newIndex)
{
    this->workPthreadIndex.store(newIndex);
    return;
}

int Client::GetWorkPthreadIndex()
{
    return this->workPthreadIndex.load();
}

void Client::UpdateClientTaskNum(int cmdTaskNum)
{
    if (cmdTaskNum >= 0)
    {
        this->workPthreadSameClientTaskNum.fetch_add(cmdTaskNum);
        return;
    }
    if (cmdTaskNum < 0)
    {
        cmdTaskNum = abs(cmdTaskNum);
        this->workPthreadSameClientTaskNum.fetch_sub(cmdTaskNum);
    }
    
    return;
}

int Client::GetClientTaskNum()
{
    return this->workPthreadSameClientTaskNum.load();
}

void Client::SetRegisterCode(int code)
{
    this->registerCode = code;
    return;
}

void Client::SetRegisterCodeTime(long time)
{
    this->registerCodeTime = time;
    return;
}

bool Client::JudgeRegisterCode(int compareCode)
{
    if (registerCode == 0 || registerCodeTime == 0)
        return false;

    long nt = Global::GetNowTime();
    if ((registerCodeTime <= nt) && (nt - registerCodeTime <= Config::registerCodeOutTimeInterval) )
    {
        if (this->registerCode == compareCode)
        {
            LOG.Log() << "注册码验证成功" << endl;
            this->SetRegisterCodeTime(0);//验证成功把验证码设为过期,避免多次使用
            return true;
        }
        LOG.Log() << "验证码不匹配" << endl;
        return false;
    }
    LOG.Log() << "验证码有效时间已过,请重新申请" << endl;
    this->SetRegisterCode(0);
    this->SetRegisterCodeTime(0);
    this->SetEmailAddress("");
    return false;
}

void Client::SetEmailAddress(std::string address)
{
    this->emailAddress = address;
}

std::string Client::GetEmailAddress()
{
    return std::string(this->emailAddress);
}

Client* Client::GetMyself()
{
    return this->pMyself;
}

string Client::GetClientUid()
{
    return this->uid;
}
