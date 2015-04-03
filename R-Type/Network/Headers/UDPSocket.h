#ifndef _UDPSOCKET_H_
# define _UDPSOCKET_H_

# define UDP_PORT 6666

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_ON_WARNINGS
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdexcept>

class UDPSocket
{
public:
	explicit UDPSocket(unsigned short port, bool doBind = true);
	~UDPSocket();


	void send(struct sockaddr_in *dest, char *buffer, unsigned int length);
	void recv(char *buffer, unsigned int length);
	void shutdown_udp();

#ifdef _WIN32
	SOCKET getSocket() const;
#else
	int getSocket() const;
#endif

private:
	struct sockaddr_in _self;

/////
/////
#ifdef _WIN32 // Sockets Windows
private:
	SOCKET _socket;
	WSADATA _wsa;
#else // Sockets Unix
private:
	int _socket;
#endif
/////
/////

};

#endif
