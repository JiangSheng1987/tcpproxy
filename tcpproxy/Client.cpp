
#include "Client.h"
#include "Socket.h"
#include "SocketHeaders.h"
#include "FunctionFailedException.h"
#include "HelperFunctions.h"

#include <iostream>
#include <stdexcept>

using namespace tcpproxy;

Socket* Client::ConnectToServer(const std::string& hostname, int port)
{
    addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	std::string portStr = std::to_string(port);
	int rc = getaddrinfo(hostname.c_str(), portStr.c_str(), &hints, &servinfo);
	if (rc != 0)
	{
		throw FunctionFailedException("getaddrinfo", rc);
	}

	int sockfd = 0;

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1)
		{
			continue;
		}

		int rc = connect(sockfd, p->ai_addr, p->ai_addrlen);
		if (rc == -1)
		{
            HelperFunctions::CloseSocket(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		throw std::runtime_error("Client: Failed to connect");
	}

	char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, HelperFunctions::GetInAddr((sockaddr*)p->ai_addr), s, sizeof s);

    printf("Client: Connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	Socket* socket = new Socket(sockfd);
	return socket;
}
