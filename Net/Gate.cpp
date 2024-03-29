#include "Gate.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
extern ClassServer* SERVER_OBJECT;

/*(ctrl + \ )*/
void Gate::ServerQuit(int signum)
{
    //LOG.Log() << "收到信号信息 = " << signum << std::endl;

    ::SERVER_OBJECT->SetIsServerDowning(true);  //停止接受客户端连接以及任何协议内容

    ::DATABASEMGR.SaveOffLineData();            //保存离线数据
    
    ::DATABASEMGR.SaveAllClientData();         //关服主动保存每个客户端数据进数据库

    ::DATABASEMGR.SaveAllPublicData();         //关服保存每个公共模块数据进数据库
    
    exit(0);
}

//信号处理准备工作
void Gate::SignalReady()
{
    // signal(SIGINT, process_exit);
    // signal(SIGFPE, process_exit);
    // signal(SIGILL, process_exit);
    // signal(SIGABRT, process_exit);
    // signal(SIGSEGV, process_exit);
    // signal(SIGTERM, process_exit);
    signal(SIGQUIT, ServerQuit);
}

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
bool Gate::JudegeEmailBrandNew(const char* tarEmailAddress, ClassDataBase* db, int socket)
{
    bool result = true;
    std::string tip = "";

    //处理mysql语句中的字符
    std::string emailaddress(DBCommand::JudegeEmailBrandNew);
    emailaddress += '"';
    emailaddress += tarEmailAddress;
    emailaddress += '"';
    //LOG.Log() << "The emailAdress is :" << emailaddress << std::endl;

    //执行mysql语句
    bool resCheck = db->DoCommand(emailaddress);
    if (resCheck != true)
    {
        LOG.Error() << "JudegeEmailBrandNew resCheck is :" << resCheck << std::endl;
        result = false;
        tip = "JudegeEmailBrandNew DBCommand Fail";
    }
    int resRow = db->GetResultRow();
    if (resRow > 0)
    {
        LOG.Log() << "该邮箱已经注册过了" << std::endl;
        result = false;
        tip = "Email has been registered,please use another one";
    }

    //返回协议(错误才返回，因为这里仅仅是判断是否注册了，还没对数据库进行插入操作,不能返回true的协议内容)
    if (result != true)
    {
        Global::MakeSendPackage* pack = new Global::MakeSendPackage("GATE", "s_registered_token_respond");
        pack->SetVal("Result", result);
        pack->SetVal("Reason", tip);
        pack->Flush(socket);
        delete pack;
    }

    return result;
}

//验证码请求
bool Gate::GetRegisteredToken(void* cliptr, const char* tarEmailAddress)
{
    bool result = true;
    std::string tip = "";
    int socket = ((Client*)cliptr)->GetClientFd();

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
        //发送成功设置验证码信息
        //LOG.Log() << "SendEmail success" << std::endl;
        ((Client*)cliptr)->SetRegisterCode(code);
        ((Client*)cliptr)->SetRegisterCodeTime(nTime);
        ((Client*)cliptr)->SetEmailAddress(std::string(tarEmailAddress));
    } 
    else
    {
        LOG.Error() << "SendEmail failed,errno = " << ret << std::endl;
        tip = "SendEmail Failed";
        result = false;
    }

    //返回协议
    Global::MakeSendPackage* pack = new Global::MakeSendPackage("GATE", "s_registered_token_respond");
    pack->SetVal("Result", result);
    pack->SetVal("Reason", tip);
    pack->Flush(socket);
    delete pack;

    return result;
}

//注册请求
//验证成功以及验证码过期会重置验证码和验证时间,但是验证码不匹配则不重置,给用户保留多次重输机会
bool Gate::Registered(void* cliptr, std::string account, std::string pw, int code, ClassDataBase* db)
{
    bool res = true;
    bool resRegister = ((Client*)cliptr)->JudgeRegisterCode(code);
    if (resRegister == false)
    {
        res = false;
    }
    else
    {
        /*注册成功则保存信息,用来验证登录*/
        std::string makeAccount = DBCommand::MakeAccount;
        std::string emailAddress = ((Client*)cliptr)->GetEmailAddress();
        makeAccount.insert(makeAccount.size() - 1, std::string("'" + account + "',"));       //账号
        makeAccount.insert(makeAccount.size() - 1, std::string("'" + pw + "',"));            //密码
        makeAccount.insert(makeAccount.size() - 1, std::string("'" + emailAddress + "',"));  //邮箱地址
        emailAddress.erase(emailAddress.size() - 7, emailAddress.size() - 1);                //去除@qq.com
        makeAccount.insert(makeAccount.size() - 1, std::string("'" + emailAddress + "'"));   //玩家UID（取邮箱前面的数值部分）
        //执行MYSQL语句
        res = db->DoCommand(makeAccount);
        if (res != true)
        {
            LOG.Error() << "Create Account result is :" << res << std::endl;
            LOG.Log() << "myslq插入数据失败" << std::endl;
        }
        else
        {
            ((Client*)cliptr)->SetRegisterCode(0);
            ((Client*)cliptr)->SetRegisterCodeTime(0);
            ((Client*)cliptr)->SetEmailAddress("");
        }
    }
    
    //返回协议
    Global::MakeSendPackage* pack = new Global::MakeSendPackage("GATE", "s_registered_respond");
    pack->SetVal("Result", res);
    pack->Flush(((Client*)cliptr)->GetClientFd());
    delete pack;

    return res;
}

std::string Gate::CheckoutAccountPassword(std::string account, std::string pw, ClassDataBase* db)
{
    std::string dbString = DBCommand::CheckoutPassword;
    dbString.insert(dbString.find('=') + 2, account);
    dbString.insert(dbString.find_last_of('=') + 2, pw);
    //LOG.Log() << "dbString = " << dbString << std::endl;
    bool res = db->DoCommand(dbString);
    if (res != true)
    {
        LOG.Error() << "SomeThing Wrong With CheckoutAccountPassword`s DBString ! " << std::endl;
        return "";
    }
    else
    {
        int row = db->GetResultRow();
        int count = db->GetResultCount();
        //LOG.Log() << "row = " << row << std::endl;
        //LOG.Log() << "count = " << count << std::endl;
        if (row <= 0 || count <= 0)
        {
            LOG.Log() << "Login Fail With Password not right !" << std::endl;
            return "";
        }
        else if(row == 1 && count == 1)
        {
            //LOG.Log() << "Login Success !" << std::endl;
            std::string actorID = (*(db->GetNextRowInfo()))[0];
            //LOG.Log() << "actorID = " << actorID << std::endl;
            return actorID;
        }
    }
    return "";
}

bool Gate::Login(std::string account, std::string pw, ClassDataBase* db, void* cliptr, LuaVmMgr* luaVmMgrPtr, void* sockidmapPtr, void* sockfdmapPtr, ClassServer* classServerPtr)
{
    //先验证账号密码
    int fd = ((Client*)cliptr)->GetClientFd();
    bool result = true;
    std::string actorId = Gate::CheckoutAccountPassword(account, pw, db);
    if (actorId.size() <= 0)
    {
        result = false;
    }
    else
    {
        //检测是否有残旧数据需要删除（有可能之前掉线了, 导致没有正常下线, 以至于数据还残存）
        CheckoutReLogin(actorId, luaVmMgrPtr, sockidmapPtr, sockfdmapPtr, classServerPtr);

        ((Client*)cliptr)->SetClientUid(actorId);                                           //设置Uid（因为验证账号密码成功后，会根据玩家的UID创建Vm虚拟机，以Uid作为VmMap的索引）
        bool resCreateLuaVm = Gate::CreateLuaVmAfterLogin(cliptr, luaVmMgrPtr, db);         //登录成功则尝试创建虚拟机
        
        if (resCreateLuaVm != true) //即使登录成功但创建失败也要跳过虚拟机操作(返回false)
        { 
            result = false; 
        }                                     
        else                        //登录成功且创建Vm成功后,才可以加入socketIdMap中,并且赋值GLOBAL_UID_SOCKET_MAP 和 GLOBAL_UID_REDISOBJECT_MAP
        { 
            //创建RedisObj*
            Redis* redisObj = new Redis();
            result = redisObj->connect(Config::redisHost, Config::redisPort);
            if (result)
            {
                //LOG.Log() << "连接Redis数据库成功:" << actorId << std::endl;
                result = Gate::RedisLoadMysqlDataByLogin(actorId, luaVmMgrPtr, redisObj, db);
            }
            else 
            {
                LOG.Log() << "连接Redis数据库失败:" << actorId << std::endl;
            }

            if (result)
            {
                ::GLOBAL_UID_REDISOBJECT_MAP.update(actorId, redisObj);

                Gate::AddIntoSockIdMap(cliptr, sockidmapPtr);

                ::GLOBAL_UID_SOCKET_MAP.update(actorId, fd);
            }
            else
            {
                LOG.Log() << "actorID = " << actorId << " ,加载Redis 数据失败" << std::endl;
                luaVmMgrPtr->DeleteLuaBaseVm(actorId);
                delete redisObj;
            }

            //LOG.Log() << "GLOBAL_UID_SOCKET_MAP 's Size is " << ::GLOBAL_UID_SOCKET_MAP.size() << endl;
            //LOG.Log() << "GLOBAL_UID_REDISOBJECT_MAP 's Size is " << ::GLOBAL_UID_REDISOBJECT_MAP.size() << endl;
        }         
    }

    //返回协议
    Global::MakeSendPackage* pack = new Global::MakeSendPackage("GATE", "s_login_respond");
    pack->SetVal("Result", result);
    pack->Flush(fd);
    delete pack;

    return result;
}

//Create Client LuaVm
bool Gate::CreateLuaVmAfterLogin(void* cliptr, LuaVmMgr* luaVmMgrPtr, ClassDataBase* db)
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
            bool resInit = L->Init(path), resLoad = true;
            if (resInit == true)
            {
                //加载数据库数据进入Lua中(旧逻辑是登录的时候把数据保存进LuaVM，现在优化为登录的时候把数据保存到Redis，所以暂时屏蔽保留这一步)
                //resLoad = Gate::LuaVmLoadMysqlDataByLogin(uid, luaVmMgrPtr, L->GetLuaStatePtr(), db); 
            }
            else
            {
                //LOG.Error() << "Personal Moudle Init Fail fd : " << uid << std::endl;
                delete L;
                return false;
            }

            if (resLoad == true)
            {
                //LOG.Log() << "Personal Moudle resLoad Success fd : "<< uid << std::endl;
                bool resAddLuaBaseVm = luaVmMgrPtr->AddLuaBaseVm(uid, (LuaBaseVm*)L);
                if (resAddLuaBaseVm == false)
                {
                    //LOG.Error() << "Personal Moudle AddLuaBaseVm Fail fd : " << uid << std::endl;
                    delete L;
                    return false;
                }
            }
            else
            {
                //LOG.Error() << "Personal Moudle resLoad Fail fd : " << uid << std::endl;
                delete L;
                return false;
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

bool Gate::LuaVmLoadMysqlDataByLogin(std::string uid, LuaVmMgr* luaVmMgrPtr, lua_State* L, ClassDataBase* db)
{
    lua_settop(L, 0);
    lua_getglobal(L, "LoadDbData_");
    Global::LuaMoudleFilesInfo* filesInfoPtr = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr(); //根据文件加载情分类况加载DB数据
    std::string dbString = "";
    for (auto it = filesInfoPtr->GetMoudleInfo()->begin(); it != filesInfoPtr->GetMoudleInfo()->end(); it++)
    {
        if (it->second.first == Global::PERSONAL)
        {
            std::string moudle = it->first;
            for (int index = 0; index < moudle.size(); index++)
            {
                moudle[index] = tolower(moudle[index]);
            }

            dbString = DBCommand::LoadLuaDataFromMysql;
            dbString.insert(dbString.find('.'), moudle);
            dbString.insert(dbString.find('.', dbString.find('.') + 1) + 1, moudle);
            dbString.insert(dbString.find_last_of('.'), moudle);
            dbString.insert(dbString.find_last_of('=') + 2, uid);
            //LOG.Log() << "Mysql Load Lua Json's dbString = " << dbString << std::endl;

            bool doCommandResult = db->DoCommand(dbString);
            if (!doCommandResult)
            {
                continue;
            }
            else
            {
                int row = db->GetResultRow();
                if (row == 1)
                {
                    std::string jsonMysqlDataString = (*(db->GetNextRowInfo()))[0];
                    moudle = moudle + "::" + uid + "::" + jsonMysqlDataString;
                    lua_pushstring(L, moudle.c_str());
                }
                else
                    continue;
            }
        }
        else
        {
            continue;
        }
    }
    int size = lua_gettop(L) - 1;
    lua_pcall(L, size, 1, 0);
    bool res = lua_isboolean(L, -1);
    if ((res != true) || (lua_toboolean(L, -1) != true))
    {
        res = false;
    }
    //LOG.Log() << "LuaVmLoadMysqlDataByLogin res = " << res << std::endl;
    return res;
}

bool Gate::RedisLoadMysqlDataByLogin(std::string uid, LuaVmMgr* luaVmMgrPtr, Redis* redisObj, ClassDataBase* db)
{
    Global::LuaMoudleFilesInfo* filesInfoPtr = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr(); //根据文件加载情分类况加载DB数据
    std::string dbString = "";
    for (auto it = filesInfoPtr->GetMoudleInfo()->begin(); it != filesInfoPtr->GetMoudleInfo()->end(); it++)
    {
        if (it->second.first == Global::PERSONAL)
        {
            std::string moudle = it->first;
            for (int index = 0; index < moudle.size(); index++)
            {
                moudle[index] = tolower(moudle[index]);
            }

            //判断是否已经在缓存有数据了，有则直接跳过不需要要再从数据库中取出了
            if (redisObj->checkoutData(uid + "_" + moudle) == true)
            {
                //LOG.Log() << "Mysql Load Check Data In Redis = " << uid + "_" + moudle << std::endl;
                continue;
            }

            dbString = DBCommand::LoadLuaDataFromMysql;
            dbString.insert(dbString.find('.'), moudle);
            dbString.insert(dbString.find('.', dbString.find('.') + 1) + 1, moudle);
            dbString.insert(dbString.find_last_of('.'), moudle);
            dbString.insert(dbString.find_last_of('=') + 2, uid);
            //LOG.Log() << "Mysql Load Json's dbString = " << dbString << std::endl;

            bool doCommandResult = db->DoCommand(dbString);
            if (!doCommandResult)
            {
                LOG.Log() << "RedisLoadMysqlDataByLogin Fail Moudle = " << moudle << std::endl;
                return false;
            }
            else
            {
                int row = db->GetResultRow();
                if (row == 1)
                {
                    std::string jsonMysqlDataString = (*(db->GetNextRowInfo()))[0];
                    //LOG.Log() << "Load Moudle = " << moudle << "  DataString = " << jsonMysqlDataString << std::endl << std::endl;
                    moudle = uid + "_" + moudle;
                    redisObj->set(moudle, jsonMysqlDataString);
                }
                else
                {
                    LOG.Log() << "RedisLoadMysqlDataByLogin Can't Find Info From MoudleDb = " << moudle << std::endl;
                    return false;
                }
            }
        }
        else
        {
            continue;
        }
    }
    return true;
}

void Gate::AddIntoSockIdMap(void* cliptr, void* sockmapPtr)
{
    Client* clientPtr = ((Client*)cliptr);
    std::string actorUid = clientPtr->GetClientUid();
    auto ptr = (SafeMap<Client*>*)sockmapPtr;
    if (ptr->CheckoutIfExist(actorUid) == false)
    {
        ptr->update(actorUid, clientPtr);
        //LOG.Log() << "当前pSockidMap人数为：" << ((std::map<string, Client*>*)sockmapPtr)->size() << endl;
    }
}

bool Gate::SaveLuaScriptDataIntoDB(std::string uid, LuaVmMgr* luaVmMgrPtr, lua_State* L, ClassDataBase* db)
{
    if (Config::dataSaveType != Config::data_Save_Type::LOGOUT_SAVE)
    {
        return true;
    }

    Global::LuaMoudleFilesInfo* filesInfoPtr = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr(); //根据文件加载情分类况加载DB数据
    std::string dbString = "";
    bool result = true;
    for (auto it = filesInfoPtr->GetMoudleInfo()->begin(); it != filesInfoPtr->GetMoudleInfo()->end(); it++)
    {
        if (it->second.first == Global::PERSONAL)
        {
            std::string moudle = it->first;
            for (int index = 0; index < moudle.size(); index++)
            {
                moudle[index] = tolower(moudle[index]);
            }

            //调用Lua函数返回对应模块的Redis数据
            lua_settop(L, 0);
            lua_getglobal(L, "SaveDbData_");
            lua_pushstring(L, uid.c_str());
            lua_pushstring(L, moudle.c_str());
            lua_pcall(L, 2, 1, 0);
            bool res = lua_isstring(L, -1);
            if (res == false)
            {
                LOG.Error() << "SaveDbData_ Wrong When C++ Call Lua  :" << moudle << std::endl;
                result = false;
                continue;
            }
            std::string data = lua_tostring(L, -1);

            //判断数据库是否存在数据（结果决定操作是update还是Insert）
            dbString = DBCommand::LoadLuaDataFromMysql;
            dbString.insert(dbString.find('.'), moudle);
            dbString.insert(dbString.find('.', dbString.find('.') + 1) + 1, moudle);
            dbString.insert(dbString.find_last_of('.'), moudle);
            dbString.insert(dbString.find_last_of('=') + 2, uid);
            bool doCommandResult = db->DoCommand(dbString);
            if (doCommandResult == false)
            {
                LOG.Error() << "DataBase Command Worng With Judege :" << moudle << std::endl;
                result = false;
                continue;
            }
            else
            {
                int row = db->GetResultRow();
                //LOG.Log() << "Mysql Load Lua Json's row = " << row << std::endl;
                if (row == 1)   //找到了就是证明有数据（update）
                {
                    dbString = DBCommand::SaveLuaDataWithUpdate;
                    dbString.insert(dbString.find('.') + 1, moudle);
                    dbString.insert(dbString.find('.', dbString.find('.') + 1), moudle);
                    dbString.insert(dbString.find('=') + 2, data);
                    dbString.insert(dbString.find_last_of('.'), moudle);
                    dbString.insert(dbString.find_last_of('=') + 2, uid);
                }
                else if(row == 0)   //找不到就是证明暂无数据（insert）
                {
                    dbString = DBCommand::SaveLuaDataWithInsert;
                    dbString.insert(dbString.find('.') + 1, moudle);
                    dbString.insert(dbString.find('\'') + 1, uid);
                    dbString.insert(dbString.find_last_of('\''), data);
                }
                else
                {
                    result = false;
                    continue;
                }
            }

            //LOG.Log() << "Mysql Load Lua Json's dbString = " << dbString << std::endl;
            doCommandResult = db->DoCommand(dbString);
            if (!doCommandResult)
            {
                LOG.Error() << "SaveDbData_ Wrong With  :" << dbString << std::endl;
                result = false;
                continue;
            }
        }
    }
    return result;
}

bool Gate::SaveRedisDataIntoDB(std::string uid, LuaVmMgr* luaVmMgrPtr, ClassDataBase* db)
{
    if (Config::dataSaveType != Config::data_Save_Type::LOGOUT_SAVE)
    {
        return true;
    }

    bool result = true;
    Redis* redisObj = nullptr;
    if (::GLOBAL_UID_REDISOBJECT_MAP.CheckoutIfExist(uid) == false)
    {
        LOG.Error() << "Can't Find User :" << uid << std::endl;
        return result;
    }
    else
    {
        redisObj = ::GLOBAL_UID_REDISOBJECT_MAP.at(uid);
    }

    Global::LuaMoudleFilesInfo* filesInfoPtr = luaVmMgrPtr->GetLuaMoudleFilesInfoPtr(); //根据文件加载情分类况加载DB数据
    std::string dbString = "";
    for (auto it = filesInfoPtr->GetMoudleInfo()->begin(); it != filesInfoPtr->GetMoudleInfo()->end(); it++)
    {
        if (it->second.first == Global::PERSONAL)
        {
            std::string moudle = it->first;
            for (int index = 0; index < moudle.size(); index++)
            {
                moudle[index] = tolower(moudle[index]);
            }

            //判断数据库是否存在数据（结果决定操作是update还是Insert）
            dbString = DBCommand::LoadLuaDataFromMysql;
            dbString.insert(dbString.find('.'), moudle);
            dbString.insert(dbString.find('.', dbString.find('.') + 1) + 1, moudle);
            dbString.insert(dbString.find_last_of('.'), moudle);
            dbString.insert(dbString.find_last_of('=') + 2, uid);
            bool doCommandResult = db->DoCommand(dbString);
            if (doCommandResult == false)
            {
                LOG.Error() << "DataBase Command Worng With Judege :" << moudle << std::endl;
                result = false;
                continue;
            }
            else
            {
                std::string data = redisObj->get(uid + "_" + moudle);
                if (data.size() == 0)
                {
                    continue;
                }

                int row = db->GetResultRow();
                //LOG.Log() << "Mysql Load Json's row = " << row << std::endl;
                if (row == 1)   //找到了就是证明有数据（update）
                {
                    dbString = DBCommand::SaveLuaDataWithUpdate;
                    dbString.insert(dbString.find('.') + 1, moudle);
                    dbString.insert(dbString.find('.', dbString.find('.') + 1), moudle);
                    dbString.insert(dbString.find('=') + 2, data);
                    dbString.insert(dbString.find_last_of('.'), moudle);
                    dbString.insert(dbString.find_last_of('=') + 2, uid);
                }
                else if (row == 0)   //找不到就是证明暂无数据（insert）
                {
                    dbString = DBCommand::SaveLuaDataWithInsert;
                    dbString.insert(dbString.find('.') + 1, moudle);
                    dbString.insert(dbString.find('\'') + 1, uid);
                    dbString.insert(dbString.find_last_of('\''), data);
                }
                else
                {
                    result = false;
                    continue;
                }
            }

           // LOG.Log() << "Mysql Load Lua Json's dbString = " << dbString << std::endl;
            doCommandResult = db->DoCommand(dbString);
            if (!doCommandResult)
            {
                LOG.Error() << "SaveDbData_ Wrong With  :" << dbString << std::endl;
                result = false;
                continue;
            }
            else
            {
                //删除Redis中的相关缓存（节省空间）,模式为下线保存数据则保存完毕后清理缓存
                if (Config::dataSaveType == Config::data_Save_Type::LOGOUT_SAVE)
                {
                    redisObj->release(uid, moudle);
                }
            }
            
        }
    }

    return result;
}

void Gate::RemoveFromSockIdMap(void* cliptr, void* sockmapPtr, std::string uid)
{
    auto ptr = (SafeMap<Client*>*)sockmapPtr;
    if (ptr->CheckoutIfExist(uid))
    {
        ptr->erase(uid);
        Client* clientp = ((Client*)cliptr);
        delete clientp;//释放Client*内存
        //LOG.Log() << "当前pSockidMap人数为：" << ptr->size() << endl;
    }
}

void Gate::CheckoutReLogin(std::string uid, LuaVmMgr* luaVmMgrPtr, void* sockidmapPtr, void* sockfdmapPtr, ClassServer* classServerPtr)
{
    std::map<string, Client*>* pSockfdMap = (std::map<string, Client*>*)sockfdmapPtr;
    SafeMap<Client*>* pSockidMap = (SafeMap<Client*>*)sockidmapPtr;
    Client* pClient = nullptr;

    //移除idMap内旧数据
    if (pSockidMap->CheckoutIfExist(uid))
    {
        //LOG.Log() << "有旧数据残留,删除旧数据：" << uid << endl;
        pClient = pSockidMap->at(uid);
        pSockidMap->erase(uid);
    }

    //移除LuaVm
    luaVmMgrPtr->DeleteLuaBaseVm(uid);

    //移除fdMap内数据
    if (pClient)
    {
        int clientFd = pClient->GetClientFd();
        //LOG.Log() << "clientFd == ：" << clientFd << endl;
        close(clientFd);
        std::string fd = std::to_string(clientFd);
        if (pSockfdMap->find(fd) != pSockfdMap->end())
        {
            pSockfdMap->erase(fd);
        }
        epoll_ctl(classServerPtr->GetEpollFd(), EPOLL_CTL_DEL, clientFd, NULL);
        classServerPtr->ChangeClientAmount(-1);
    }

    //移除UID-Socket的键值对容器内的数据
    if (::GLOBAL_UID_SOCKET_MAP.CheckoutIfExist(uid))
    {
        ::GLOBAL_UID_SOCKET_MAP.erase(uid);
        //LOG.Log() << "GLOBAL_UID_SOCKET_MAP [clientFd] == ：" << (::GLOBAL_UID_SOCKET_MAP)[uid] << endl << endl;
    }

    //移除UID-Redis*的键值对容器内的数据
    if (::GLOBAL_UID_REDISOBJECT_MAP.CheckoutIfExist(uid))
    {
        Redis* redisObj = ::GLOBAL_UID_REDISOBJECT_MAP.at(uid);
        ::GLOBAL_UID_REDISOBJECT_MAP.erase(uid);
        delete redisObj;
        redisObj = nullptr;
        //LOG.Log() << "GLOBAL_UID_REDISOBJECT_MAP [clientFd] == ：" << (::GLOBAL_UID_REDISOBJECT_MAP)[uid] << endl << endl;
    }

    //析构client指针所指向的内存
    delete pClient;
    return;
}
