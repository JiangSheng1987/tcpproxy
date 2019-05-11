
#ifndef __helper_functions_h__
#define __helper_functions_h__

struct sockaddr;

namespace tcpproxy {

class HelperFunctions
{
public:
    static void InitSocketLib();
    static void* GetInAddr(sockaddr* sa);
    static void CloseSocket(int fd);
};

} // namespace tcpproxy

#endif
