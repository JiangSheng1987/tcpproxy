
#ifndef __client_h__
#define __client_h__

#include <string>

namespace tcpproxy {

class Socket;

class Client
{
public:
    Socket* ConnectToServer(const std::string& hostname, int port);
};

} // namespace tcpproxy

#endif
