#!/bin/bash

function ServerSatrt()
{
    local testPath="../Logger/LogInfo"
    #判断文件夹是否存在 -d
    if [[ ! -d "$testPath" ]]; then
        mkdir ../Logger/LogInfo
    fi

    local testFile="../Logger/LogInfo/main_Logger.txt"
    #判断文件是否存在
    if [ -e "$testFile" ]; then
        > ../Logger/LogInfo/main_Logger.txt
    else
        touch ../Logger/LogInfo/main_Logger.txt
    fi

    local result=$(lsof -i:8888 | cat | tail -n 1)
    
    if [ "$result" == "" ]; then
        $Purple && echo "开始启动服务器........." && $RESET
        echo " "
        ./Server.out > ../Logger/LogInfo/main_Logger.txt
    else
        $RED && echo "服务器已启动，请勿重复启动" && $RESET
        echo "  "
    fi
}

function ServerStop()
{
    #端口为8888的进程数据，并获取最后一行(可以用 ps代替lsof 但是ps可能会包含本来的搜索命令的关键字)
    local result=$(lsof -i:8888 | cat | tail -n 1)

    if [ "$result" == "" ]; then
        $RED && echo "暂无服务器可以关闭" && $RESET
        echo " "
    else
        declare -a arr
        index=0
        for i in $(echo $result | awk '{print $1,$2}')
        do
            arr[$index]=$i
            let "index+=1"
        done
        pid=${arr[1]}
        $GREENBLUE && echo "开始关闭服务器........." && $RESET
        $GREENBLUE && echo "服务器进程PID: "$pid  && $RESET
        echo " "
        kill -3 $pid
    fi
    
}

function ServerReboot()
{
    ServerStop
    ServerSatrt
}

function DoCmd()
{
    if [ "$param" == "start" ]; then
        ServerSatrt
    elif [ "$param" == "stop" ]; then
        ServerStop
    elif [ "$param" == "reboot" ]; then
        ServerReboot
    else
        $RED && echo "Input Wrong!"  && $RESET
    fi

}

RED="echo -en \\E[0;31m"
GREEN="echo -en \\E[0;32m"
YELLOW="echo -en \\E[0;33m"
Purple="echo -en \\E[0;35m"
GREENBLUE="echo -en \\E[0;36m"
RESET="echo -en \\E[0;39m"

#参数（1.start 启动 2.stop 关闭 3.reboot 重启）
param=$1

#调用函数DoCmd+参数
DoCmd $param
