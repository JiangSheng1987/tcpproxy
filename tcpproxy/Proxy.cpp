
#include "Proxy.h"
#include "Client.h"
#include "Server.h"
#include "Socket.h"
#include "HelperFunctions.h"

#include <thread>
#include <stdexcept>

using namespace tcpproxy;

Proxy::Proxy(
    int listenPort,
    const std::string forwardHostname,
    int forwardPort,
    std::function<void(char*, int)> beforeSendToServer /* = nullptr */,
    std::function<void(char*, int)> beforeSendToClient /* = nullptr */) :

    m_listenPort(listenPort),
    m_forwardHostname(forwardHostname),
    m_forwardPort(forwardPort),
    m_beforeSendToServer(beforeSendToServer),
    m_beforeSendToClient(beforeSendToClient)
{

}

void Proxy::Run()
{
    HelperFunctions::InitSocketLib();

    Server server([this](Socket* serverSocket)
    {
        try
        {
            Client client;
            Socket* clientSocket = client.ConnectToServer(m_forwardHostname, m_forwardPort);

            std::thread clientToServer([this, clientSocket, serverSocket]()
            {
                this->ForwardData(clientSocket, serverSocket, m_beforeSendToServer);
            });

            std::thread serverToClient([this, clientSocket, serverSocket]()
            {
                this->ForwardData(serverSocket, clientSocket, m_beforeSendToClient);
            });

            clientToServer.join();
            serverToClient.join();

            clientSocket->Close();
            serverSocket->Close();

            delete clientSocket;
            delete serverSocket;
        }
        catch (const std::exception& ex)
        {
            printf("Exception: %s\n", ex.what());
        }
    });

    server.Listen(m_listenPort);
}

void Proxy::ForwardData(Socket* firstSocket, Socket* secondSocket, std::function<void(char*, int)> beforeSend /* = nullptr */)
{
    try
    {
        while (true)
        {
            char buf[65535];
            int bytesReceived = firstSocket->Receive(buf, 1024);
            printf("Forwarding %d bytes . . .\n", bytesReceived);

            if (beforeSend != nullptr)
            {
                beforeSend(buf, bytesReceived);
            }

            int bytesSent = secondSocket->Send(buf, bytesReceived);
        }
    }
    catch (const std::exception& ex)
    {
        printf("Exception: %s\n", ex.what());
    }
}
