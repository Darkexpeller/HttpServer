#include "CreateTcpSocket.h"
#include "HttpHandle.h"
#include "is_error.h"
#include <time.h>
#include <filesystem>
#include"recvclient.h"
#include<signal.h>
#include<thread>
// 192.168.219.128:9005
using std::cout;
using std::endl;
int *psockfd = NULL;


static void handle_term(int sig)
{
	shutdown(*psockfd,SHUT_WR);
	printf("Programe is exiting...\n");
	exit(sig);
}
int main()
{
	signal(SIGTERM, handle_term);
	signal(SIGINT, handle_term); // Ctrl+C will send SIGINT signal;
	// const char *mes = "GET http://www.baidu.com/ HTTP/1.1\r\n"
	// 				  "Host: www.baidu.com\r\n"
	// 				  "Connection: keep-alive\r\n\r\n";
	// std::string recv;
	// HttpHandle::HttpSend(mes, &recv, -1);
	// cout << recv;




	clock_t start, finish;
	const char *filename = "1.txt";

	FILE *fo = fopen(filename, "w+");
	int sockfd = CreateServerTcpSocket(9005); // in_port
	psockfd = &sockfd;
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	// 接受客户端
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	bzero(&client_addr, sizeof(client_addr));
	printf("begin to accept\n");
	int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
	while (true)
	{
		
		// int client_sockfd = accept(sockfd, NULL, NULL);

		// char buffer[1024];
		// bzero(buffer, sizeof(buffer));
		printf("有接入\n");
		std::string Cbuffer;
		while(true)
		{
			Cbuffer.clear();
			RecvClientMessage(client_sockfd, Cbuffer);
			std::cout << Cbuffer << std::endl;
		}

		
		// recv(client_sockfd, buffer, sizeof(buffer), 0);
		// fprintf(fo, buffer);
		// printf("%s", buffer);
	}

	return 1;
}