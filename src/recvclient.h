#ifndef RECVCLIENT_H
#define RECVCLIENT_H
#include<iostream>
struct MesHeader
{
    char Flag[5] = {'M','E','S','S','\0'};
    int Length = 0;
};

int RecvClientMessage(int sockfd, std::string &content);

#endif