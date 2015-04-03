#include <cstring>
#include "UDPSocket.h"

UDPSocket::UDPSocket(unsigned short port, bool doBind)
{
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &(this->_wsa)) == SOCKET_ERROR)
		throw std::runtime_error("WSAStartup failed");
#endif
	if ((this->_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	  	throw std::runtime_error("Socket failed");

	this->_self.sin_family = AF_INET;
	this->_self.sin_addr.s_addr = INADDR_ANY;
	this->_self.sin_port = htons(port);

	if (doBind)
	{
		if (bind(this->_socket, (struct sockaddr*)&_self, sizeof(_self)) < 0)
		{
			throw std::runtime_error("Binding failed");
		}
	}
}

UDPSocket::~UDPSocket()
{
}

void UDPSocket::send(struct sockaddr_in *dest, char *buffer, unsigned int length)
{
	if (sendto(_socket, buffer, length, 0, (struct sockaddr*)dest, sizeof(*dest)) == -1)
	{
		throw std::runtime_error("sendto failed");
	}
}

void UDPSocket::recv(char *buffer, unsigned int length)
{
	if (recvfrom(_socket, buffer, length, 0, 0, 0) <= 0)
	{
		throw std::runtime_error("recvfrom failed");
	}
}

void UDPSocket::shutdown_udp()
{
#ifdef _WIN32
	closesocket(this->_socket);
#else
	shutdown(this->_socket, SHUT_RDWR);
	close(this->_socket);
#endif
}

#ifdef _WIN32
SOCKET UDPSocket::getSocket() const
{
	return _socket;
}
#else
int UDPSocket::getSocket() const
{
	return _socket;
}
#endif