#include"outputlog.h"

#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include <cstdlib>
#include<chrono>
#include<vector>
LogRecode::Log::Log()
{
    this->InitLog();
}
void LogRecode::Log::InitLog()
{
    system("mkdir -p ./log/");
    pLogfile = fopen(LogFileDir,"a");
    if(!pLogfile )
    {
        printf("The log system faled to initialize.\n");
        exit(0);
    }
    Logmutex = false;
    IsInit = true;
}
void LogRecode::Log::OutputToLog(const char* str)
{
     if(IsInit==false)
     {
         InitLog();
     }
    // while(Logmutex==true)
    // {
    //     usleep(10000);// pause 10 ms
    // }
    // Logmutex = true;
    printf("begin to log\n");
    // 获取当前系统时间点
    auto now = std::chrono::system_clock::now();

    // 将时间点转换为时间戳
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // 使用 localtime 将时间戳转换为本地时间
    std::tm *local_tm = std::localtime(&now_time);

    // 获取年月日时分秒
    int year = local_tm->tm_year + 1900;   // 年份从1900年开始计数
    int month = local_tm->tm_mon + 1;      // 月份从0开始计数
    int day = local_tm->tm_mday;
    int hour = local_tm->tm_hour;
    int minute = local_tm->tm_min;
    int second = local_tm->tm_sec;
    std::string TimeOutPut = std::to_string(year) + "/" + std::to_string(month) + "/" + std::to_string(day) + "/ " + ((hour<10)?"0"+std::to_string(hour):std::to_string(hour)) + ":" + ((minute<10)?"0"+std::to_string(minute):std::to_string(minute)) + ":" + ((second<10)?"0"+std::to_string(second):std::to_string(second)) + ": "+str+'\n';
    fprintf(pLogfile, TimeOutPut.c_str());
    fflush(pLogfile);
}
void LogRecode::Log::LogThreadHanle(const char* str)
{
    this->Log_Lock.lock();
    OutputToLog(str);

    this->Log_Lock.unlock();
}
LogRecode::Log LogOut;
//std::thread LogSystemThread(LogRecode::Log::LogThreadHanle, std::ref(LogOut));