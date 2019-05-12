
#ifndef __server_h__
#define __server_h__

#include <functional>
#include <string>

namespace tcpproxy {

class Socket;

class Server
{
public:
    Server(std::function<void(Socket*)> connectionHandler);

    void Listen(int port);

private:
    int CreateAndBindServerSocket(int port);

    std::function<void(Socket*)> m_connectionHandler;
};

} // namespace tcpproxy

#endif
