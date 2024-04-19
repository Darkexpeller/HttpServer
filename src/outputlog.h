#ifndef OUTPUTLOG_H
#define OUTPUTLOG_H
#include <filesystem>
#include<thread>
#include<mutex>
namespace LogRecode
{
    struct DataToThread
    {
        
    };
    class Log
    {
        public:
        Log();
    private:
        const char *LogFileDir = "./log/Log.txt";
        bool Logmutex = 0; // Ensure the log be operated only by one.
        FILE *pLogfile;
        bool IsInit = false;
        std::mutex Log_Lock;
        void OutputToLog(const char *);
        void InitLog();
    public:
        
        void LogThreadHanle(const char*);
       
    };

}
extern LogRecode::Log LogOut;
#endif