#include "CreateTcpSocket.h"
#include "HttpHandle.h"
#include "is_error.h"
#include <time.h>
#include <filesystem>
#include"recvclient.h"
#include<signal.h>
#include<thread>
#include"outputlog.h"
#include<arpa/inet.h>
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

void TestConstChar(const char *str) // Don;t Transfer point or left value parm to asyn function
{
	sleep(2);
	printf("%s\n", str);
	return;
}


void demo2()
{
	signal(SIGTERM, handle_term);
	signal(SIGINT, handle_term); // Ctrl+C will send SIGINT signal;




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
	char Ipstr[INET_ADDRSTRLEN];
	memset(Ipstr, 0, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), Ipstr, INET_ADDRSTRLEN); //Transfer uint_32 to IP_str
	std::string Loginfo("INFO - Receive a client: IP: ");
	Loginfo += Ipstr;
	LogOut.LogThreadHanle(Loginfo.c_str());

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
}


void demo3()
{
	const char *str = "Nihao\n";
	std::thread Test(TestConstChar,str);
	
	delete str;
	Test.join();
	//Test.detach();
}
int main()
{
	demo2();
	//demo3();
	// 	// shutdown(sockfd, 2);
	// 	start = clock();
	// 	finish = clock();
	// 	cout << "seocnd= " << (double)(finish - start) / CLOCKS_PER_SEC << "s " << endl;


	//  Test HTTP message


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


