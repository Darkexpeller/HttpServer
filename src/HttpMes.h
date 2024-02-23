#pragma once

//to handle and parse http message
#include <iostream>
#include<map>
#include<vector>
#include<cstring>
#include<sstream>
#include<stdlib.h>
#include<errno.h>    
#define RESPONSETYPE
using std::stringstream;
using std::map;
using std::string;
using std::vector;

class HttpMes
{
public:
	HttpMes(string mes, int _FD); 
	//bool Response(char* mes,int _FqD=socket_fd);   set this function in HttpResponse
	void HttpParseHeader();
	void Extractcontent(string::iterator it);
	map<string,string> HttpMap;//::iterator it->first,second.....
	string GetHeadersValue(string key ="");
private:
	string message;
    int socket_fd;
	vector<string> Header;
	string content;
};