#include"recvclient.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include <string.h>
int RecvClientMessage(int sockfd, std::string &content)
{
    MesHeader headerbuffer;
    memset(&headerbuffer, 0, sizeof(MesHeader));
    recv(sockfd, &headerbuffer, sizeof(headerbuffer), MSG_WAITALL);
   //std::cout << headerbuffer.size;
    if (0 == strcmp(headerbuffer.Flag, "MESS"))
    {
        char *ContentBuffer = new char[headerbuffer.Length];
        memset(ContentBuffer, 0, headerbuffer.Length);
        int q = recv(sockfd, ContentBuffer, headerbuffer.Length, MSG_WAITALL);
        std :: cout << q << std::endl;
        if (q == 0)
        {
            exit(0);
        }
        content += ContentBuffer;
        return headerbuffer.Length;
    }
    else
    {
        exit(0);
        return -1;
    }
}