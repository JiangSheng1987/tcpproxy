
#ifndef __socket_h__
#define __socket_h__

namespace tcpproxy {

class Socket
{
public:
	Socket(int fd);
	virtual ~Socket();

	int Send(const char* buf, int len);
	int Receive(char* buf, int len);
	void Close();

private:
	int m_fd;
};

} // namespace tcpproxy

#endif
