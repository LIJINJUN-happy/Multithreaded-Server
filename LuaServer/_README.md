Lua 文档描述：

LUA 模块部分是通过Lua虚拟机来与C++交互的
Lua虚拟机分为 “个人类型” 以及 “公共类型” 

“个人类型”：个人类型的Lua虚拟机用来加载个人lua模块,这些模块每个用户登录成功后都会有自己的模块字段,存在于用户的虚拟机内
例如：每个用户都有自己Bag模块,个人虚拟机之间相互独立。
（该虚拟机会在玩家登录,加载数据成功后进行创建）


“公共类型”：该类型的虚拟机是公用的,每个用户都可以对虚拟机进行操作,公共模块为每个用户创建了一个协程（corotinue）来进行操作。
若不同虚拟机之间想交互,则要通过返回特定的数据,来调用别的虚拟机.
（该类虚拟机会在服务器启动的时候进行创建）


模块定义在该文件同目录下的名为“MoudleFilesLoad.txt”的文本文件内，只需要按规定填写上新建的模块信息即可：
格式：{模块名（全大写）,模块的文件路径}
例：{ROOM,LuaServer/LuaSystem/Room/roomMgr.lua}


Lua虚拟机与C++之间传递的参数数据类型为：JSON格式,因为Mysql5.7.xxx后支持JSON格式数据的存储
为了统一性,协议,数据库,以及Lua信息的传递均用JSON格式处理.

Lua虚拟机之间若要传递信息,则返回固定的格式作为corotinue的挂起返回值
如：
coroutine.yield(xxx,xxx,xx,xx,xx,x)
返回参数分别是：
std::string arg = "";   //参数
std::string fun = "";   //调用函数/协议
std::string called = "";//被调用的模块VM名字
std::string caller = "";//调用者,一开始调用者模块默认为空
std::string uid = "";   //调用者id



该项目的Lua部分的要求是,需要在Linux下载LuaRock管理工具,然后用管理工具下载 Lua-Redis 来作为lua数据的缓存