#include "HttpHandle.h"
using namespace std;
HttpHandle::HttpMes::HttpMes(string mes, int _FD) : message(mes), socket_fd(_FD),is_http{false}
{
	HttpParseHeader();
}
void HttpHandle::HttpMes::Extractcontent(string::iterator it)
{
	while (it != message.end())
	{
		content += *it;
	}
}
void HttpHandle::HttpMes::HttpParseHeader()
{
	string Line_temp;
	string::iterator it = message.begin();
	while (it != message.end() || *(Header.begin() + Header.size() - 1) != "\r\n")
	{
		if (*it != '\n')
		{
			Line_temp += *it; // Adds the *it to the string
		}
		else
		{
			Line_temp += *it;
			Header.push_back(Line_temp); // Adds the Line_temp to the vector
			Line_temp.clear();			 // clear temp to parse next line
		}
		it++;
	}
	Header.pop_back(); // delete the blank line
	vector<string>::iterator pHeaderLine = Header.begin();
	string::iterator pFirstLine = (*pHeaderLine).begin();

	// Handle the first line RequestType, ProtocolType, ProtocolVersion

	string RequestType, URL,End, ProtocolType, ProtocolVersion ;
	// The "End" variable is declared here to read ProtocolType and ProtocolVersion at once
	string *OperatorList[] = {&RequestType, &URL, &End, &ProtocolType, &ProtocolVersion}; // An array of Pointers is declared to simplify the code
	string **pOperator = OperatorList;
	std::cout << "FirstLine=" << *pHeaderLine << std::endl;
	// If you can ensure the HTTP message's format is ture,some condition can be ignored to simplify code
	while (pFirstLine != pHeaderLine->end() && *pFirstLine != '\r' && *pOperator != OperatorList[3])
	{
		std::cout << "*pFirstLine=" << *pFirstLine << std::endl;
		if (*pFirstLine ==' ')
		{
			pFirstLine ++;
			pOperator++;
			continue;
		}
		**pOperator += *(pFirstLine++);
	}
	std::cout << "End=" << End << std::endl;
	pOperator++;
	for (string::iterator i = End.begin(); i != End.end(); i++)
	{
		//std::cout << "*i=" << *i << std::endl;
		std::cout << "**pOperator=" << **pOperator << std::endl;
		if (*i == '/')
		{
			pOperator++;
		}
		else{
			**pOperator += *i;
		}
		
	}
	HttpMap["RequestType"] = RequestType;
	HttpMap["URL"] = URL;
	HttpMap["ProtocolType"] = ProtocolType;
	HttpMap["ProtocolVersion"] = ProtocolVersion;
	if(ProtocolType=="HTTP")          // 如果不是http报文，则return函数，且讲is_http设置为false;
	{
		is_http = true;
	}
	else{
		is_http = false;
		return; 
	}
	// Handle the header to the map
	pHeaderLine++;
	string key, value;
	size_t position = 0;

	while (pHeaderLine != Header.end())
	{
		position = pHeaderLine->find_first_of(':');

		// if found
		if (position != pHeaderLine->npos)
		{
			for (int i = 0; i < position; i++)
			{
				key += (*pHeaderLine)[i];
			}
			position += 2;
			for (; position < pHeaderLine->length() && (*pHeaderLine)[position] != '\r'; position++)
			{
				value += (*pHeaderLine)[position];
			}
			HttpMap[key] = value;
			key.clear();
			value.clear();
			pHeaderLine++;
		}

		else // not found':'
		{
			pHeaderLine++;
		}
	}
}
string HttpHandle::HttpMes::GetHeadersValue(string key)
{
	string value;
	if (key == "")  
	{
		vector<string>::iterator ThisLine = Header.begin();
		while (ThisLine != Header.end())
		{
			value += *ThisLine;
			ThisLine++;
		}
	}
	else
	{
		map<string, string>::iterator iter = HttpMap.find(key);
		if (iter == HttpMap.end())
		{
			std::cout<<"Not "<<key<<" in headers";
			value = "";
		}
		else if(iter->second=="")
		{
			std::cout << "key " << key << " is blank";
			value = "";
		}
		else if(iter->second!="")
		{
			value = iter->second;
		}
	}
	return value;
}
