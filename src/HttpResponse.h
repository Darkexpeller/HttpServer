#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include"HttpMes.h"


class HttpResponse
{
public:
    HttpResponse(HttpMes ResponseMessage, int sockfd);//向sockfd发送HTTP响应报文
};

#endif