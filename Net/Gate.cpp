#include "Gate.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

//转换为char*然后send
void Gate::TransformationAndSend(string msg)
{
    //信息长度
    int msgSize = msg.size();
    // LOG.Log() << "打包的信息字符串长度为：" << msgSize << endl;
    if (msgSize <= 0)
    {
        return;
    }

    //信息长度值的长度
    string sizeLenString = std::to_string(msgSize);
    int sizeLen = sizeLenString.size();
    // LOG.Log() << "大小值" << msgSize << "的长度为" << sizeLen << endl;

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
        LOG.Log() << buf[i];
    }
    LOG.Log() << endl;
    */
    return;
}

//接受recv的数据data然后分解里面有多少协议
vector<string> Gate::GetAndTransformation(string msg)
{
    // LOG.Log() << "获取到了整体msg:  " << msg << "  总长度为：" << msg.size() << endl;
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
        // LOG.Log() << "获取字节的位数：" << strMgsTotalNum << endl;

        //获取信息的长度
        string sizeLenString(msg, 1, strMgsTotalNum);
        int sizeLen = std::stoi(sizeLenString);
        deleteTotal += sizeLen;
        // LOG.Log() << "信息长度为：" << sizeLen << endl;

        //获取信息并存入msgtable
        string realString(msg, strMgsTotalNum + 1, sizeLen);
        msgTable.push_back(realString);

        //更新msg
        msg.erase(msg.begin(), msg.begin() + deleteTotal);
    }
    return msgTable;
}

int Gate::GetRandByTimes(int from, int to, int times)
{
    //设置随机种子
    int tm = (int)time(NULL);
    srand(tm);

    int res = 0;
    for (int i = 0; i < times; i++)
    {
        int value = (rand() % (to - from + 1)) + from;
        value *= (pow(10,i));
        res += value;
    }
    return res;
}

//是否为未注册的邮箱
bool Gate::JudegeEmailBrandNew(const char* tarEmailAddress, ClassDataBase* db)
{
    std::string emailaddress(DBCommand::JudegeEmailBrandNew);
    emailaddress += '"';
    emailaddress += tarEmailAddress;
    emailaddress += '"';
    //LOG.Log() << "The emailAdress is :" << emailaddress << std::endl;
    bool resCheck = db->DoCommand(emailaddress);
    if (resCheck != true)
    {
        LOG.Error() << "JudegeEmailBrandNew resCheck is :" << resCheck << std::endl;
        return false;
    }
    int resRow = db->GetResultRow();
    if (resRow > 0)
    {
        LOG.Error() << "JudegeEmailBrandNew resRow is :" << resRow << std::endl;
        return false;
    }
    return true;
}

//验证码请求
bool Gate::GetRegisteredToken(void* cliptr, const char* tarEmailAddress)
{
    //生产随机的数字组合
    int code = Gate::GetRandByTimes(1, 9, Config::registerCodeSize);
    long nTime = Global::GetNowTime();
    std::string codeString = "The Register Code Is : ";
    codeString += std::to_string(code);
    //const char* cstring = codeString.c_str();

    //发送到邮箱
    int ret = SendEmail(
        "smtp.qq.com",
        25,
        "2849533742@qq.com",
        "uzuvsixtwjpxdffa",
        //"2231173990@qq.com",
        tarEmailAddress,
        codeString.c_str(),
        "LIJINJUN_SERVER",
        "Actor",
        "Register Code");
    if (ret == 0)
    {
        //LOG.Log() << "SendEmail success" << std::endl;
    } 
    else
    {
        LOG.Error() << "SendEmail failed,errno = " << ret << std::endl;
        return false;
    }
       

    //发送成功设置验证码信息
    ((Client*)cliptr)->SetRegisterCode(code);
    ((Client*)cliptr)->SetRegisterCodeTime(nTime);
    ((Client*)cliptr)->SetEmailAddress(std::string(tarEmailAddress));
    return true;
}

//注册请求
//验证成功以及验证码过期会重置验证码和验证时间,但是验证码不匹配则不重置,给用户保留多次重输机会
bool Gate::Registered(void* cliptr, std::string account, std::string pw, int code, ClassDataBase* db)
{
    bool resRegister = ((Client*)cliptr)->JudgeRegisterCode(code);
    if (resRegister == false)
    {
        return false;
    }

    /*注册成功则保存信息,用来验证登录*/
    std::string makeAccount = DBCommand::MakeAccount;
    makeAccount.insert(makeAccount.size() - 1, std::string("'" + account + "',"));
    makeAccount.insert(makeAccount.size() - 1, std::string("'" + account + "',"));
    makeAccount.insert(makeAccount.size() - 1, std::string("'" + pw + "',"));
    makeAccount.insert(makeAccount.size() - 1, std::string("'" + ((Client*)cliptr)->GetEmailAddress() + "'"));
    bool res = db->DoCommand(makeAccount);
    if (res != true)
    {
        LOG.Error() << "Create Account result is :" << res << std::endl;
        return false;
    }

    ((Client*)cliptr)->SetRegisterCode(0);
    ((Client*)cliptr)->SetRegisterCodeTime(0);
    ((Client*)cliptr)->SetEmailAddress("");
    return true;
}

bool Gate::Login(int fd, void* fdMapPtr, std::string account, std::string pw)
{
    return true;
}

//Create Client LuaVm
bool Gate::CreateLuaVmAfterLogin(void* cliptr, LuaVmMgr* luaVmMgrPtr)
{
    std::string uid = ((Client*)cliptr)->GetClientUid();
    //std::map<std::string, LuaBaseVm*>* luaVmMapPtr = luaVmMgrPtr->GetLuaVmMapPtr();
    //auto luaVmMapPtrIterator = luaVmMapPtr->find(uid);
    bool isExist = luaVmMgrPtr->CheckLuaVmIsExistByIndex(uid);
    if (isExist == false)
    {
        //新建一个VM
        std::string path = luaVmMgrPtr->GetPathByStringFromFilesInfo("ACTOR");
        //LOG.Log() << "Actor Path = " << path << std::endl;
        if (path.size() >= 1)
        {
            LuaPersonalVm* L = new LuaPersonalVm(Global::PERSONAL, uid);
            bool resLoad = L->Init(path);
            if (resLoad == true)
            {
                //LOG.Log() << "Personal Moudle Init Success fd : "<< uid << std::endl;
                luaVmMgrPtr->AddLuaBaseVm(uid, (LuaBaseVm*)L);
            }
            else
            {
                delete L;
                return false;
                //continue;
            }
        }
    }
    else
    {
        //vm已存在
        return false;
    }
    return true;
}

void Gate::AddIntoSockIdMap(void* cliptr, void* sockmapPtr)
{
    std::string actorUid = ((Client*)cliptr)->GetClientUid();
    auto it = ((std::map<string, Client*>*)sockmapPtr)->find(actorUid);
    if (it == ((std::map<string, Client*>*)sockmapPtr)->end())
    {
        ((std::map<string, Client*>*)sockmapPtr)->insert(std::make_pair(actorUid, (Client*)cliptr));
        LOG.Log() << "当前pSockidMap人数为：" << ((std::map<string, Client*>*)sockmapPtr)->size() << endl;
    }
}

void Gate::RemoveFromSockIdMap(void* cliptr, void* sockmapPtr, std::string uid)
{
    auto it = ((std::map<string, Client*>*)sockmapPtr)->find(uid);
    if (it != ((std::map<string, Client*>*)sockmapPtr)->end())
    {
        ((std::map<string, Client*>*)sockmapPtr)->erase(uid);
        Client* clientp = ((Client*)cliptr)->GetMyself();
        delete clientp;//释放Client*内存
        LOG.Log() << "当前pSockidMap人数为：" << ((std::map<string, Client*>*)sockmapPtr)->size() << endl;
    }
}
