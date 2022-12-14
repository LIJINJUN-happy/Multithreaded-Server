#!/bin/bash

dir=$(pwd)
echo "当前路径为:$dir"
echo

#所需链接
needLink=" -lpthread -std=c++11 -L/usr/lib64/mysql -lmysqlclient"
echo "所需链接:$needLink"
echo

#main文件
main_dir=" $dir/*.cpp"
echo "主路径下文件:$main_dir"
echo

#数据库文件
DB_dir=" $dir/ClassDB/*.cpp "
echo "数据库类文件:$DB_dir"
echo

#线程类文件
pthread_dir=" $dir/ClassPthread/*.cpp "
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

all_Dir="$needLink$main_dir$DB_dir$pthread_dir$timer_dir$Net_dir$Monitor_dir"
echo "g++执行$all_Dir"
g++ $all_Dir -o ./cmd/Server.out
echo