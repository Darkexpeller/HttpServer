#include "HttpHandle.h"
#include <sys/socket.h>
#include "is_error.h"
int HttpHandle::HttpSend(std::string mes, int _FD)
{
    if (HttpHandle::HttpMes(mes, 0).IsHttpMessage())
    {
        is_error(send(_FD, mes.c_str(), mes.size(), 0) == -1, NULL);
        return 1;
    }
    else
    {
        printf("A mes is not htpp message in fd: %d", _FD);
    }
}

int HttpHandle::HttpMes::HttpParseURL(HttpHandle::HttpMes &mes, HttpHandle::Url& content) // 这个函数应该在需要的时候再调用，而不是对每一个http都调用
                                                                               // 以免造成资源浪费
                                                                               // 去除协议，域名，端口后的path
                                                                               // 这个方法比较全面，但操作偏多，适合健全性需求较强场景
                                                                               // 对于可控的http通讯大可简化，使用另外一个版本
{
    std::string URL = mes.GetHeadersValue("URL");
    std::string Scheme, DomainName, Port, Path, Parm, Anchor;
    std::string *OperatorList[] = {&Scheme, &DomainName, &Port, &Path, &Parm, &Anchor};
    std::string *pOperator = &content.Scheme;
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
        pOperator++;
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