#ifndef _NETUSER_H_
# define _NETUSER_H_

#ifdef _WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <stdexcept>

class NetUser
{
public:
	NetUser();
	~NetUser();
	std::string getIP() const;
	struct sockaddr_in *getSockaddr();

#ifdef _WIN32
	SOCKET getSocket();
	void setSocket(SOCKET);
#else
	int getSocket();
	void setSocket(int);
#endif

private:
	struct sockaddr_in _sockaddr;

#ifdef _WIN32
	SOCKET _socket;
#else
	int _socket;
#endif
};

#endif