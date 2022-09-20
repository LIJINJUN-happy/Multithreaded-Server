#!/bin/bash

function ServerSatrt()
{
    result=$(lsof -i:8888 | cat | tail -n 1)
    
    if [ "$result" == "" ]; then
        echo "开始启动服务器........."
        echo " "
        ./Server.out
    else
        echo "服务器已启动，请勿重复启动"
        echo "  "
    fi
}

function ServerStop()
{
    #端口为8888的进程数据，并获取最后一行(可以用 ps代替lsof 但是ps可能会包含本来的搜索命令的关键字)
    result=$(lsof -i:8888 | cat | tail -n 1)

    if [ "$result" == "" ]; then
        echo "暂无服务器可以关闭"
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
        echo "开始关闭服务器........."
        echo "服务器进程PID= "$pid
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
        echo "Input Wrong!"
    fi

}

#参数（1.start 启动 2.stop 关闭 3.reboot 重启）
param=$1

#调用函数DoCmd+参数
DoCmd $param