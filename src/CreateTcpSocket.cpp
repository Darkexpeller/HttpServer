#include"CreateTcpSocket.h"
#include"is_error.h"
#include<errno.h>
#include"outputlog.h"
#include<string>
SOCKET_FILE_D CreateServerTcpSocket(int in_port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	is_error(sockfd==-1,"socket is fail");
	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(in_port);//1024 and smaller need root privilege

	is_error(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1,"bind is error");
	// switch (ReturnCode)
	// {
	// case EACCES:
	// 	printf("EACCES error; permission denied.\n");
	// 	break;
	// case EADDRINUSE:
	// 	printf("Address has been used.");
	// 	break;
	// default:
	// 	printf("An error has occurred in bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));"
	// 			"The Code is %d",ReturnCode)
	// 	 break;
	// }
	listen(sockfd, SOMAXCONN); // int listen(int sockfd,int backlog);
	LogOut.LogThreadHanle(std::string("INFO - Server's socket is created.The listen port is "+std::to_string(in_port)).c_str());
	return sockfd;
}