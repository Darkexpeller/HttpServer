#include"HttpHandle.h"
#include<sys/socket.h>
#include"is_error.h"
int HttpHandle::HttpSend(std::string  mes,int _FD)
{
    if(HttpHandle::HttpMes(mes,0).IsHttpMessage())
    {
        is_error(send(_FD, mes.c_str(), mes.size(),0)==-1,NULL);
        return 1;
    }
    else
    {
        printf("A mes is not htpp message in fd: %d", _FD);
    }
}

int HttpHandle::HttpGetResource(HttpHandle::HttpMes &mes, std::string content)
{
    std::string URL = mes.GetHeadersValue("URL");
    
}