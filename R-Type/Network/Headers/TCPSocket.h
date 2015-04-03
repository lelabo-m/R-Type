#ifndef _TCPSOCKET_H_
# define _TCPSOCKET_H_

# define TCP_PORT 6666

#ifdef _WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdexcept>
#include <string>

class NetUser;

class TCPSocket
{
public:
	enum TCPState
	{
		CONNECTED,
		CONNECTING,
		DISCONNECTED
	};

private:
	enum SocketState
	{
		HOSTING,
		NOT_HOSTING
	};

public:
	explicit TCPSocket(unsigned short port);
	~TCPSocket();

#ifdef _WIN32
	void sendTCP(SOCKET sock, char *buffer, unsigned int length);
	void recvTCP(SOCKET sock, char *buffer, unsigned int length);
	SOCKET getSocket();
#else
	void sendTCP(int sock, char *buffer, unsigned int length);
	void recvTCP(int sock, char *buffer, unsigned int length);
	int getSocket();

#endif
	void sendToServer(char *buffer, unsigned int length);
	void recvFromServer(char *buffer, unsigned int length);

	void shutdown();
	bool connectTo(const std::string &ip);
	void host();

	NetUser *acceptClient();

	TCPState getSocketState() const;
	struct sockaddr_in getSockaddrIn();

private:
	struct sockaddr_in _self;
	TCPState _state;
	SocketState _socketState;

private:
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