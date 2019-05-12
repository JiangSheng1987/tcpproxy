
#include "Server.h"
#include "Socket.h"
#include "SocketHeaders.h"
#include "FunctionFailedException.h"
#include "HelperFunctions.h"

#include <thread>
#include <iostream>

#define BACKLOG 1000

using namespace tcpproxy;

Server::Server(std::function<void(Socket*)> connectionHandler)
{
    m_connectionHandler = connectionHandler;
}

void Server::Listen(int port)
{
    int sockfd = this->CreateAndBindServerSocket(port);

    int rc = listen(sockfd, BACKLOG);
    if (rc == -1)
    {
        throw FunctionFailedException("listen", rc);
    }

    printf("Server: Waiting for connections . . .\n");

    sockaddr_storage clientaddr; // connector's address information

    while (true)
    {
        socklen_t sinsize = sizeof clientaddr;
        int newfd = accept(sockfd, (sockaddr*)&clientaddr, &sinsize);
        if (newfd == -1)
        {
            continue;
        }

        char s[INET6_ADDRSTRLEN];
        inet_ntop(clientaddr.ss_family, HelperFunctions::GetInAddr((sockaddr*)&clientaddr), s, sizeof s);
        std::string clientAddrStr(s);
        printf("Server: Got connection from %s\n", clientAddrStr.c_str());

        Socket* socket = new Socket(newfd);

        std::thread connection([this, socket]()
        {
            this->m_connectionHandler(socket);
        });

        connection.detach();
    }
}

int Server::CreateAndBindServerSocket(int port)
{
    addrinfo hints, *servinfo, *p;
	
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    std::string portStr = std::to_string(port);
    int rc = getaddrinfo(NULL, portStr.c_str(), &hints, &servinfo);
    if (rc != 0)
    {
        throw FunctionFailedException("getaddrinfo", rc);
    }

    int sockfd = 0;

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
        {
            continue;
        }

        int yes = 1;
        int rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int));
        if (rc == -1)
        {
            freeaddrinfo(servinfo);
            throw FunctionFailedException("setsockopt", rc);
        }

        rc = bind(sockfd, p->ai_addr, p->ai_addrlen);
        if (rc == -1)
        {
            HelperFunctions::CloseSocket(sockfd);
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        throw std::runtime_error("Server: Failed to bind");
    }

    return sockfd;
}
