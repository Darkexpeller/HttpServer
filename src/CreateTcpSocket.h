#pragma once

#include <sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<stdio.h>
#include<unistd.h>

#define SOCKET_FILE_D int
#define BIND_ERROR -1

SOCKET_FILE_D CreateServerTcpSocket(int in_port);


