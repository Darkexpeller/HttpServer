#ifndef HttpHandle_H
#define HttpHandle_H

#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <type_traits>
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

    struct HttpHeader
    {
        Url URLstruct;
    };
    struct HTTPResponseHeader
    {
        std::string httpVersion;
        int statusCode;
        std::string statusMessage;
        std::vector<std::pair<std::string, std::string>> headers;

        std::string toString() const
        {
            std::string headerString;
            headerString += httpVersion + " " + std::to_string(statusCode) + " " + statusMessage + "\r\n";
            for (const auto &header : headers)
            {
                headerString += header.first + ": " + header.second + "\r\n";
            }
            headerString += "\r\n";
            return headerString;
        }
    };

    class HttpBase
    {
    public:
            
    };

        class HttpMes // To parse Http message
    {
    public:
        HttpMes(std::string mes, int _FD);
       // HttpMes(HttpHandle::HttpHeader *HeaderStruct = NULL, const char *URL = NULL, const char *Cookie = NULL, const char *Set_Cookie = NULL, const char *);
        void HttpParseHeader();
        void Extractcontent(std::string::iterator it);
        std::map<std::string, std::string> HttpMap; //::iterator it->first,second.....
        std::string GetHeadersValue(std::string key = "");
        const char *c_mes();
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
    int HttpSend(std::string mes, std::string *pRecv = NULL, int _FD = -1); // If you want to set proxy, please offer sockfd;
                                                                            // Otherwise,function will send to "Host";
    int HttpResponse(std::string mes, int _FD = -1);                        // If you want to set proxy, please offer sockfd;
                                                                            // Otherwise,function will send to "Host";
    // int HttpParseURL(HttpMes &mes,Url &content);//
};
#endif