#ifndef HttpHandle_H
#define HttpHandle_H


#include <iostream>
#include<map>
#include<vector>
#include<cstring>
#include<type_traits>
namespace HttpHandle
{
    struct Url
    {
        std::string Scheme;
        std::string DomainName;
        std::string Port;
        std::string Path;
        std::string Parm;
        std::string Anchor;
    };
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
        int HttpParseURL(Url &content);
        int GetPort();
        const char *GetHostName(); // there is no port Parm
    private:
        std::string message;
        int socket_fd;
        bool is_http;
        std::vector<std::string> Header;
        std::string content;
        bool IsHaveParsedURL = 0;
        HttpHandle::Url URLstruct;
        
    };
    int HttpSend(std::string mes,std::string *pRecv=NULL, int _FD=-1);
    //int HttpParseURL(HttpMes &mes,Url &content);//

};
#endif