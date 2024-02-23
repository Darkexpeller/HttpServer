#ifndef HttpHandle_H
#define HttpHandle_H


#include <iostream>
#include<map>
#include<vector>
#include<cstring>
#include<type_traits>
namespace HttpHandle
{
    
    class HttpMes // To parse Http message
    {
    public:
        HttpMes(std::string mes, int _FD);
        void HttpParseHeader();
        void Extractcontent(std::string::iterator it);
        std::map<std::string,std:: string> HttpMap; //::iterator it->first,second.....
        std::string GetHeadersValue(std::string key = "");
        const char *GetMes();
        bool IsHttpMessage();
    private:
        std::string message;
        int socket_fd;
        bool is_http;
        std::vector<std::string> Header;
        std::string content;
    };
    int HttpSend(std::string mes, int _FD);
    int HttpGetResource(HttpMes &mes,std::string &content);//
};
#endif