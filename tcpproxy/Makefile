all: tcpproxy

tcpproxy: Client.o HelperFunctions.o main.o Proxy.o Server.o Socket.o
	g++ -std=c++11 Client.o HelperFunctions.o main.o Proxy.o Server.o Socket.o -o tcpproxy -lpthread

Client.o: Client.cpp
	g++ -std=c++11 -c Client.cpp

HelperFunctions.o: HelperFunctions.cpp
	g++ -std=c++11 -c HelperFunctions.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

Proxy.o: Proxy.cpp
	g++ -std=c++11 -c Proxy.cpp

Server.o: Server.cpp
	g++ -std=c++11 -c Server.cpp

Socket.o: Socket.cpp
	g++ -std=c++11 -c Socket.cpp

clean:
	rm -rf *.o