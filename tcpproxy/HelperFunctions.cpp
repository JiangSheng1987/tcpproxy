
#include "HelperFunctions.h"
#include "SocketHeaders.h"
#include "FunctionFailedException.h"

#include <string>

using namespace tcpproxy;

void HelperFunctions::InitSocketLib()
{
    #ifdef WIN_ENV

        static bool initialized = false;

        if (!initialized)
        {
            WSADATA wsaData;
            int rc = WSAStartup(MAKEWORD(1, 1), &wsaData);
            if (rc != 0)
            {
                throw FunctionFailedException("WSAStartup", rc);
            }

            initialized = true;
        }

    #endif
}

void* HelperFunctions::GetInAddr(sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((sockaddr_in*)sa)->sin_addr);
    }

    return &(((sockaddr_in6*)sa)->sin6_addr);
}

void HelperFunctions::CloseSocket(int fd)
{
    #ifdef WIN_ENV
        closesocket(fd);
    #else
        close(fd);
    #endif
}
