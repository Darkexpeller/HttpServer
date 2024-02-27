//因为结构变化，命名不要在意

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
		else{
			**pOperator += *(pFirstLine++);
		}
		
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



int HttpHandle::HttpMes::HttpParseURL(Url &content) // 这个函数应该在需要的时候再调用，而不是对每一个http都调用
                                                                               // 以免造成资源浪费
                                                                               // 去除协议，域名，端口后的path
                                                                               // 这个方法比较全面，但操作偏多，适合健全性需求较强场景
                                                                               // 对于可控的http通讯大可简化，使用另外一个版本
{

	if(IsHaveParsedURL ==true)
	{
		content = URLstruct;
		return 1;
	}
	IsHaveParsedURL = true;
    std::string URL =GetHeadersValue("URL");
	if(URL=="")
	{
		return 0;
	}
    // std::string Scheme, DomainName, Port, Path, Parm, Anchor;
    // std::string *OperatorList[] = {&Scheme, &DomainName, &Port, &Path, &Parm, &Anchor};
    std::string *pOperator = &URLstruct.Scheme;
    std::size_t SchemeFlagPosition = URL.find_first_of("://");
    std::size_t ParmFlagPosition = URL.find_first_of("?");
    std::size_t AnchorFlagPosition = URL.find_first_of("#");

    std::size_t BeginPosition = 0;
    std::size_t preEndposition = 0;
    std::size_t Endposition = SchemeFlagPosition; // 找到是否有协议域;
                                                  // 会不会其他地方出现://？？？
    if (Endposition == URL.npos)
    {
        Endposition = 0;
        pOperator++;
    }
    else
    {

        for (int i = 0; i < Endposition; i++)
        {
            *pOperator += URL[i];
        }
        BeginPosition = Endposition + 3; // 改变搜索的开始位置;
        pOperator++;                     // 更改操作的字符串;
    }

    // 接下来一定是域名
    preEndposition = Endposition;
    Endposition = URL.find_first_of('/', BeginPosition);
    if (Endposition == BeginPosition) // 这里等于BeginPosition主要是因为有可能没有域名，直接就是从根目录开始(往往伴随的是连协议都没有)
    {
        pOperator+=2;
        Endposition = preEndposition;
    }
    else
    { // 有域名的
        // 有些不是默认80端口访问的可能会带上端口
        for (int i = BeginPosition; i < Endposition; i++)
        {
            if (URL[i] != ':')
            {
                *pOperator += URL[i];
            }
            else
            {
                pOperator++;
            }
        }
        BeginPosition = Endposition + 1; // 改变搜索的开始位置;
        // 在这里处理Port
        pOperator++; // 更改操作的字符串;
    }
    // 在这里处理path
    // Endposition = URL.find_first_of('?',BeginPosition);
    for (int i = BeginPosition; i < URL.size(); i++)
    {
        if (URL[i] == '?' || URL[i] == '#')
        {
            BeginPosition = i;
            pOperator++;
            break;
        }
        else
        {
            *pOperator += URL[i];
        }
    }
    if (BeginPosition == ParmFlagPosition) // 遇到了?
    {
        for (int i = (++BeginPosition); i < URL.size(); i++)
        {
            if (URL[i] == '#')  //解析完参数遇到了#
            {

                BeginPosition = i;
                pOperator++;

                for (int j = BeginPosition + 1; j < URL.size(); j++)
                {
                    *pOperator += URL[i];
                }
				return 1; //解析完锚点后解析结束
            }
            else{
                *pOperator += URL[i];
            }
        }
    }
    else{  //遇到的是#而不参数标识‘？’
        for (int i = (++BeginPosition); i < URL.size(); i++)
        {
            if (URL[i] == '#')
            {
                BeginPosition = i;
                pOperator++;
                break;
            }
            else{
                *pOperator += URL[i];
            }
        }
        return 1; //解析完锚点后解析结束
    }

    //
}