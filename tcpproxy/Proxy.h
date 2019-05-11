
#ifndef __proxy_h__
#define __proxy_h__

#include <string>
#include <functional>

namespace tcpproxy {

class Socket;

class Proxy
{
public:
    Proxy(
        int listenPort,
        const std::string forwardHostname,
        int forwardPort,
        std::function<void(const char*, int)> beforeSendToServer = nullptr,
        std::function<void(const char*, int)> beforeSendToClient = nullptr);

    void Run();

private:
    void ForwardData(Socket* firstSocket, Socket* secondSocket, std::function<void(char*, int)> beforeSend = nullptr);

    int m_listenPort;
    std::string m_forwardHostname;
    int m_forwardPort;
    std::function<void(const char* buf, int len)> m_beforeSendToServer;
    std::function<void(const char* buf, int len)> m_beforeSendToClient;
};

} // namespace tcpproxy

#endif
