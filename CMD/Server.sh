#!/bin/bash

function ServerSatrt()
{
    echo "开始启动服务器........."
    ./Server.out
}

function ServerStop()
{
    echo "开始关闭服务器........."
    result=$(top -u lijinjun -n 1 | grep signal.out )
    declare -a arr
    index=0
    for i in $(echo $result | awk '{print $1,$2}')
    do
        arr[$index]=$i
        let "index+=1"
    done
    #echo "进程PID="${arr[0]}
    kill -s SIGQUIT ${arr[0]}
}

function ServerReboot()
{
    ServerStop
    ServerSatrt
}

function DoCmd()
{
    if [ "$param" -eq "start" ]; then
        ServerSatrt
    elif [ "$param" -eq "stop" ]; then
        ServerStop
    elif [ "$param" -eq "reboot" ]; then
        ServerReboot
    else
        echo "Input Wrong!"
    fi

}

#参数（1.start 启动 2.stop 关闭 3.reboot 重启）
param=$1

#调用函数DoCmd+参数
DoCmd $param