#include "HttpHandle.h"
#include <sys/socket.h>
#include "is_error.h"
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
using namespace std;

int HttpStreamRead(string &content, int _FD = -1)
{
    if (-1 == _FD)
    {

        return 0;
    }

    else
    {
        char recvstream = 0;
        int ReadCount = 0;
        while (ReadCount <= 2048)
        {
            recv(_FD, &recvstream, sizeof(recvstream), 0);
            content += recvstream;
            ++ReadCount;
            if (recvstream == '\n' && *(content.end() - 3) == '\n')
            {
                return content.length();
            }
            recvstream = 0;
        }
    }
    return -1;
}

int HttpHandle::HttpSend(std::string mes, std::string *pRecv, int _FD)
{

    if ("" == mes)
    {
        return 0;
    }
    HttpHandle::HttpMes http(mes, 0);
    cout << "begin send" << endl;
    const char *Hostname = http.GetHostName();
    int Port = http.GetPort();
    cout << Hostname << Port << "\n"
         << http.IsHttpMessage() << "\n";
    if (http.IsHttpMessage())
    {
        if (_FD != -1) // 提供了socket
        {
            if (send(_FD, mes.c_str(), mes.size(), 0) == -1) // 是否发送成功
            {
            }
            else
            {
                char buffer[1024];
                char StatusLine[6];
                char recvstream = 0;
                memset(StatusLine, 0, sizeof(StatusLine));
                memset(buffer, 0, sizeof(buffer));
                std::string content;
                recv(_FD, StatusLine, sizeof(StatusLine), 0); // 接收响应报文的状态行
                content += StatusLine;
                memset(StatusLine, 0, sizeof(StatusLine));
                if (content.find("HTTP") == content.npos) // 万一不是http呢？
                {
                    std::cout << "The response message from host " << Hostname << " is not HTTP." << std::endl;
                    return -2;
                }

                // while (true)
                // {
                //     recv(_FD, &recvstream, sizeof(recvstream), 0);
                //     content += buffer;
                //     memset(&recvstream, 0, sizeof(recvstream));

                //     size_t pos = content.find("\r\n\r\n");
                //     if (pos != content.npos) // 如果已经接收完了头部信息
                //     {

                //     }

                // }
                int ReturnCode = HttpStreamRead(content, _FD);
                switch (ReturnCode)
                {
                case 0:
                    printf("Not fd\n");
                    return 0;
                    break;
                case -1:
                    printf("Header is too big(>2048bytes)\n");
                    return -1;
                default:
                    break;
                }

                HttpHandle::HttpMes ResponseMes(content, 0);

                return 1;
            }
        }
        else // 没有提供fd，根据http报文来发送
        {
            clock_t start = clock();

            if (Hostname != NULL)
            {
                // 先解析出Host对应的IP地址
                struct addrinfo hints;
                struct addrinfo *result = NULL;
                std::memset(&hints, 0, sizeof(struct addrinfo));
                hints.ai_family = AF_INET; // IPv4
                hints.ai_socktype = SOCK_STREAM;
                cout << "begin send" << endl;
                int status = getaddrinfo(Hostname, NULL, &hints, &result);

                if (status != 0)
                {
                    std::cout << "Host: " << Hostname << " Not find Ipaddress" << std::endl;
                    std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
                    return 1;
                }
                else
                {

                    char ipstr[INET_ADDRSTRLEN];
                    struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
                    inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
                    std::cout << "Sending data to Host: " << ipstr << std::endl;
                    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                    if (sockfd == -1)
                    {
                        std::cout << "Host: " << Hostname << " socket creats failed" << std::endl;
                        return 0;
                    }
                    // struct sockaddr_in addr;

                    // addr.sin_family = AF_INET;
                    // //addr.sin_addr.s_addr = inet_addr(result->ai_addr);
                    // addr.sin_port = htons(Port);
                    struct sockaddr_in addr;
                    std::memcpy(&addr, result->ai_addr, sizeof(struct sockaddr_in));
                    addr.sin_port = htons(Port);
                    if (connect(sockfd, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
                    {
                        std::cout << "Host: " << Hostname << " Link failed" << std::endl;
                        return 0;
                    }
                    else
                    {
                        send(sockfd, mes.c_str(), mes.size(), 0);
                        std::cout << "SEND !!!\n";
                    }
                    char buffer[1024];
                    recv(sockfd, buffer, sizeof(buffer), 0);
                    *pRecv += buffer;

                    // return 0;
                }
            }
            else
            {
                return 0;
            }
            // 创建套接字

            clock_t finish = clock();
            std::cout << "Handle seocnd= " << (double)(finish - start) / CLOCKS_PER_SEC << "s " << std::endl;
            return 1;
        }
    }
    else
    {
        printf("A mes is not htpp message in fd: %d", _FD);
    }
}

HttpHandle::HttpMes::HttpMes(string mes, int _FD) : message(mes), socket_fd(_FD), is_http{false}
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
            Line_temp.clear();           // clear temp to parse next line
        }
        it++;
    }
    Header.pop_back(); // delete the blank line
    vector<string>::iterator pHeaderLine = Header.begin();
    string::iterator pFirstLine = (*pHeaderLine).begin();
    if (pHeaderLine->find("HTTP") == 0)
    {
        vector<string> StatusLine(3);
        vector<string>::iterator pOperator = StatusLine.begin();
        while (pFirstLine != pHeaderLine->end())
        {
            if (*pFirstLine == ' ')
            {
                pFirstLine++;
                pOperator++;
                continue;
            }
            else
            {
                *pOperator += *(pFirstLine++);
            }
        }
        HttpMap["Version"] = StatusLine[0];
        HttpMap["StatusCode"] = StatusLine[1];
        HttpMap["Reason"] = StatusLine[2];
    } // Handle the first line RequestType, ProtocolType, ProtocolVersion
    else
    {
        string RequestType, URL, End, ProtocolType, ProtocolVersion;
        // The "End" variable is declared here to read ProtocolType and ProtocolVersion at once
        string *OperatorList[] = {&RequestType, &URL, &End, &ProtocolType, &ProtocolVersion}; // An array of Pointers is declared to simplify the code
        string **pOperator = OperatorList;
        // std::cout << "FirstLine=" << *pHeaderLine << std::endl;
        //  If you can ensure the HTTP message's format is ture,some condition can be ignored to simplify code
        while (pFirstLine != pHeaderLine->end() && *pFirstLine != '\r' && *pOperator != OperatorList[3])
        {
            // std::cout << "*pFirstLine=" << *pFirstLine << std::endl;
            if (*pFirstLine == ' ')
            {
                pFirstLine++;
                pOperator++;
                continue;
            }
            else
            {
                **pOperator += *(pFirstLine++);
            }
        }
        std::cout << "End=" << End << std::endl;
        pOperator++;
        for (string::iterator i = End.begin(); i != End.end(); i++)
        {
            // std::cout << "*i=" << *i << std::endl;
            // std::cout << "**pOperator=" << **pOperator << std::endl;
            if (*i == '/')
            {
                pOperator++;
            }
            else
            {
                **pOperator += *i;
            }
        }
        HttpMap["RequestType"] = RequestType;
        HttpMap["URL"] = URL;
        HttpMap["ProtocolType"] = ProtocolType;
        HttpMap["ProtocolVersion"] = ProtocolVersion;
        if (ProtocolType == "HTTP") // 如果不是http报文，则return函数，且讲is_http设置为false;
        {
            is_http = true;
        }
        else
        {
            is_http = false;
            return;
        }
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
            std::cout << "Not " << key << " in headers";
            value = "";
        }
        else if (iter->second == "")
        {
            std::cout << "key " << key << " is blank";
            value = "";
        }
        else if (iter->second != "")
        {
            value = iter->second;
        }
    }
    return value;
}

int HttpHandle::HttpMes::HttpParseURL(Url &content) // 这个函数应该在需要的时候再调用，而不是对每一个http都调用111
                                                    // 以免造成资源浪费
                                                    // 去除协议，域名，端口后的path
                                                    // 这个方法比较全面，但操作偏多，适合健全性需求较强场景
                                                    // 对于可控的http通讯大可简化，使用另外一个版本
{

    if (IsHaveParsedURL == true)
    {
        content = URLstruct;
        return 1;
    }
    IsHaveParsedURL = true;
    std::string URL = GetHeadersValue("URL");
    if (URL == "")
    {
        return 0;
    }
    // std::string Scheme, DomainName, Port, Path, Parm, Anchor;
    // std::string *OperatorList[] = {&Scheme, &DomainName, &Port, &Path, &Parm, &Anchor};
    std::string *pOperator = &content.Scheme;
    std::size_t SchemeFlagPosition = URL.find("://");
    std::size_t ParmFlagPosition = URL.find("?");
    std::size_t AnchorFlagPosition = URL.find("#"); // 注意不要用find_first_of

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
        pOperator += 2;
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
    for (std::string::iterator i = URL.begin() + BeginPosition; i != URL.end(); ++i)
    {
        if (*i == '?' || *i == '#')
        {
            BeginPosition = i - URL.begin();
            pOperator++;
            break;
        }
        else
        {
            *pOperator += *i;
        }
        if (i == URL.end() - 1) // 只有路径没有其他部分的情况
        {
            URLstruct = content;
            return 1;
        }
    }
    if (BeginPosition == ParmFlagPosition) // 遇到了?
    {
        for (int i = (++BeginPosition); i < URL.size(); i++)
        {
            if (URL[i] == '#') // 解析完参数遇到了#
            {

                BeginPosition = i;
                pOperator++;

                for (int j = BeginPosition + 1; j < URL.size(); j++)
                {
                    *pOperator += URL[i];
                }
                URLstruct = content;
                return 1; // 解析完锚点后解析结束
            }
            else
            {
                *pOperator += URL[i];
            }
        }
    }
    else
    { // 遇到的是#而不参数标识‘？’
        for (int i = (++BeginPosition); i < URL.size(); i++)
        {
            if (URL[i] == '#')
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
        URLstruct = content;
        return 1; // 解析完锚点后解析结束
    }

    //
}
bool HttpHandle::HttpMes::IsHttpMessage()
{
    return is_http;
}
const char *HttpHandle::HttpMes::GetHostName()
{
    HttpParseURL(URLstruct);
    if (this->IsHaveParsedURL && this->URLstruct.DomainName != "") // 如果被解析过
    {
        return this->URLstruct.DomainName.c_str();
    }
    else // 这里解析Host字段
    {
        string Host = GetHeadersValue("Host");
        if (Host != "")
        {
            size_t pos = Host.find(':', 0);
            if (pos != Host.npos)
            {
                for (int i = 0; i < pos; ++i)
                {
                    URLstruct.DomainName += Host[i];
                }
                if (URLstruct.Port == "")
                {
                    for (int i = pos + 1; i < Host.size() - 1; ++i)
                    {
                        URLstruct.Port += Host[i];
                    }
                }
                return this->URLstruct.DomainName.c_str();
            }
            else
            {
                URLstruct.DomainName = Host;
                URLstruct.Port = "80"; // 默认http协议;
                return this->URLstruct.DomainName.c_str();
            }
        }
        else
        {
            return NULL;
        }
    }
}

int HttpHandle::HttpMes::GetPort()
{
    // cout << "Hellp";
    if (URLstruct.Port != "")
    {
        return std::stoi(URLstruct.Port);
    }
    else
    {
        string Host = GetHeadersValue("Host");
        if (Host != "")
        {
            size_t pos = Host.find(':', 0);
            if (pos != Host.npos)
            {
                for (int i = pos + 1; i < Host.size() - 1; ++i)
                {
                    URLstruct.Port += Host[i];
                }
                return std::stoi(URLstruct.Port);
            }
            else
            {
                if (true)
                {
                    URLstruct.Port = "80"; // 默认http协议;
                    return 80;
                }
                else
                {
                    return -1;
                }

                // return this->URLstruct.DomainName.c_str();
            }
        }
    }
}