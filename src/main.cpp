﻿#include "CreateTcpSocket.h"
#include "HttpHandle.h"
#include "is_error.h"
#include <time.h>
#include <filesystem>
// 192.168.219.128:9005
using std::cout;
using std::endl;

int main()
{

	const char *mes = "GET http://www.baidu.com/ HTTP/1.1\r\n"
					  "Host: www.baidu.com\r\n"
					  "Connection: keep-alive\r\n\r\n";
	std::string recv;
	HttpHandle::HttpSend(mes,&recv);
	cout << recv;
	// clock_t start, finish;
	// const char *filename = "1.txt";
	// FILE *fo = fopen(filename, "w+");
	// int sockfd = CreateServerTcpSocket(9005); // in_port
	// int opt = 1;
	// setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	// // 接受客户端
	// struct sockaddr_in client_addr;
	// socklen_t client_addr_len = sizeof(client_addr);
	// bzero(&client_addr, sizeof(client_addr));
	// printf("begin to accept\n");
	// while (true)
	// {
	// 	int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
	// 	// int client_sockfd = accept(sockfd, NULL, NULL);

	// 	char buffer[1024];
	// 	bzero(buffer, sizeof(buffer));
	// 	printf("有接入\n");

	// 	recv(client_sockfd, buffer, sizeof(buffer), 0);
	// 	fprintf(fo, buffer);
	// 	printf("%s", buffer);
	// 	// shutdown(sockfd, 2);
	// 	start = clock();
	// 	finish = clock();
	// 	cout << "seocnd= " << (double)(finish - start) / CLOCKS_PER_SEC << "s " << endl;
	// 	HttpHandle::HttpMes http(buffer, sockfd);
	// 	std::string value = http.GetHeadersValue("Host");
	// 	cout << "URL=" << http.GetHeadersValue("URL") << endl;
	// 	cout << "RequestType=" << http.GetHeadersValue("RequestType") << endl;
	// 	cout << "ProtocolType=" << http.GetHeadersValue("ProtocolType") << endl;
	// 	cout << "ProtocolVersion=" << http.GetHeadersValue("ProtocolVersion") << endl;

	// 	cout << value << endl;
	// 	cout << "length of value is" << value.length() << endl;
	// 	HttpHandle::Url urls;

	// 	try
	// 	{
	// 		http.HttpParseURL(urls);
	// 	}
	// 	catch (const std::exception &e)
	// 	{
	// 		cout << "Scheme=" << urls.Scheme << endl;
	// 		cout << "Domain=" << urls.DomainName << endl;
	// 		std::cerr << e.what() << '\n';
	// 	}

	// 	cout << "Scheme=" << urls.Scheme << endl;
	// 	cout << "Domain=" << urls.DomainName << endl;
	// 	cout << "Port=" << urls.Port << endl;
	// 	cout << "Path=" << urls.Path << endl;

	// 	cout << "Parm=" << urls.Parm << endl;
	// 	shutdown(sockfd, SHUT_WR); // 立即并同时关闭sockfd上的读和写
		// close(sockfd);//引用数-1
	// }

	return 1;
}