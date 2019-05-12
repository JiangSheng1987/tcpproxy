
#include "Socket.h"
#include "HelperFunctions.h"
#include "SocketHeaders.h"
#include "FunctionFailedException.h"

using namespace tcpproxy;

Socket::Socket(int fd)
{
    m_fd = fd;
}

Socket::~Socket()
{
    if (m_fd != -1)
    {
        this->Close();
    }
}

int Socket::Send(const char* buf, int len)
{
    int bytes = send(m_fd, buf, len, 0);

    if (bytes <= 0)
    {
        throw FunctionFailedException("send", bytes);
    }

    return bytes;
}

int Socket::Receive(char* buf, int len)
{
    int bytes = recv(m_fd, buf, len, 0);

    if (bytes <= 0)
    {
        throw FunctionFailedException("revc", bytes);
    }

    return bytes;
}

void Socket::Close()
{
    HelperFunctions::CloseSocket(m_fd);
}
