
#include "Proxy.h"

#include <iostream>
#include <stdexcept>

using namespace tcpproxy;

void PrintBuffer(const char* buf, int len);

int main(int argc, char** argv)
{
    try
    {
        if (argc != 4)
        {
            printf("Usage: tcpproxy listen_port forward_hostname forward_port\n");
            return 1;
        }

        std::string listenPortStr(argv[1]);
        int listenPort = std::stoi(listenPortStr);

        std::string forwardHostname(argv[2]);

        std::string forwardPortStr(argv[3]);
        int forwardPort = std::stoi(forwardPortStr);

        // You can inject your own logic instead of PrintBuffer()
        Proxy proxy(listenPort, forwardHostname, forwardPort, PrintBuffer, PrintBuffer);
        proxy.Run();

        return 0;
    }
    catch (const std::exception& ex)
    {
        printf("Exception: %s\n", ex.what());
        return 1;
    }
}

void PrintBuffer(const char* buf, int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%c ", buf[i]);
    }

    printf("\n");
}
