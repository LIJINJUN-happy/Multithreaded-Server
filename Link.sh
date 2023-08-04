#!/bin/bash

dir=$(pwd)
echo "当前路径为:$dir"
echo

#所需链接
needLink=" -lpthread -std=c++11 -L/usr/lib64/mysql -lmysqlclient -I/usr/local/include/ -L/usr/local/lib/ -lm -ljsoncpp -ljemalloc -lhiredis "
needLuaLink=" /usr/local/lib/liblua.a -ldl -lm -Wl,-rpath='/usr/local/lib/lua/5.4/socket' -Wl,-E "
echo "所需链接:$needLink"
echo "所需Lua链接:$needLuaLink"
echo

#main文件
main_dir=" $dir/*.cpp"
echo "主路径下文件:$main_dir"
echo

#离线数据处理文件
OFFLine_dir=" $dir/OffLineSystem/*.cpp "
echo "离线数据处理文件:$OFFLine_dir"
echo

#数据库文件
DB_dir=" $dir/ClassDB/*.cpp "
echo "数据库类文件:$DB_dir"
echo

#线程类文件
pthread_dir=" $dir/ClassPthreadMgr/*.cpp "
echo "线程类文件:$pthread_dir"
echo

#定时器文件
timer_dir=" $dir/ClassTimer/*.cpp "
echo "定时器文件:$timer_dir"
echo

#网络TCP类文件
Net_dir=" $dir/Net/*.cpp "
echo "网络TCP类文件:$Net_dir"
echo

#监视类文件
Monitor_dir=" $dir/Monitor/*.cpp "
echo "监视类文件文件:$Monitor_dir"
echo

#LuaVM类文件
LuaServer_dir=" $dir/LuaServer/LuaVmMgr/*.cpp "
echo "LuaVM类文件:$LuaServer_dir"
echo

#Mail类文件
Mail_dir=" $dir/Email/*.cpp "
echo "Mail邮件类文件:$Mail_dir"
echo

#Log类文件
Logger_dir=" $dir/Logger/ClassLogger/*.cpp "
echo "Logger类文件:$Logger_dir"
echo

all_Dir="$needLink$main_dir$OFFLine_dir$DB_dir$pthread_dir$timer_dir$Net_dir$Monitor_dir$LuaServer_dir$Mail_dir$Logger_dir"
echo "g++执行$all_Dir"
g++ $all_Dir -o ./cmd/Server.out -g $needLuaLink
echo