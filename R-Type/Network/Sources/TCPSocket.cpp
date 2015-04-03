#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef _WIN32
#include <unistd.h>
#endif

#include <cstring>
#include "TCPSocket.h"
#include "Session.h"
#include "NetUser.h"


TCPSocket::TCPSocket(unsigned short port)
{
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &(this->_wsa)) == SOCKET_ERROR)
		throw std::runtime_error("WSAStartup failed");
#endif
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Socket failed");

#ifdef _WIN32
	const char yes = 1;
#else
	int yes = 1;
#endif

	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	  {
	    throw std::runtime_error("setsockopt failed");
	  }
	
	this->_self.sin_family = AF_INET;
	this->_self.sin_port = htons(port);

	this->_state = TCPState::DISCONNECTED;
	this->_socketState = SocketState::NOT_HOSTING;
}


TCPSocket::~TCPSocket()
{
}

#ifdef _WIN32
SOCKET TCPSocket::getSocket()
{
	return _socket;
}

void TCPSocket::sendTCP(SOCKET sock, char *buffer, unsigned int length)
{
	int ret;
	unsigned int sent = 0;

	while (sent < length)
	{
		if ((ret = send(sock, &buffer[sent], length - sent, 0)) <= 0)
		{
			throw std::runtime_error("Send failed");
		}
		sent += ret;
	}
}

void TCPSocket::recvTCP(SOCKET sock, char *buffer, unsigned int length)
{
	int ret;
	unsigned int read = 0;

	while (read < length)
	{
		if ((ret = recv(sock, &buffer[read], length - read, 0)) <= 0)
		{
			throw std::runtime_error("Recv failed");
		}
		read += ret;
	}
}

#else
int TCPSocket::getSocket()
{
	return _socket;
}

void TCPSocket::sendTCP(int sock, char *buffer, unsigned int length)
{
	int ret;
	unsigned int sent = 0;

	while (sent < length)
	{
		if ((ret = send(sock, &buffer[sent], length - sent, 0)) <= 0)
		{
			throw std::runtime_error("Send failed");
		}
		sent += ret;
	}
}

void TCPSocket::recvTCP(int sock, char *buffer, unsigned int length)
{
	int ret;
	unsigned int read = 0;

	while (read < length)
	{
		if ((ret = recv(sock, &buffer[read], length - read, 0)) <= 0)
		{
			throw std::runtime_error("Recv failed");
		}
		read += ret;
	}
}
#endif

void TCPSocket::sendToServer(char *buffer, unsigned int length)
{
	int ret;
	unsigned int sent = 0;

	while (sent < length)
	{
		if ((ret = send(this->_socket, &buffer[sent], length - sent, 0)) <= 0)
		{
			throw std::runtime_error("Send failed");
		}
		sent += ret;
	}
}

void TCPSocket::recvFromServer(char *buffer, unsigned int length)
{
	int ret;
	unsigned int read = 0;

	while (read < length)
	{
		if ((ret = recv(this->_socket, &buffer[read], length - read, 0)) <= 0)
		{
			throw std::runtime_error("Recv failed");
		}
		read += ret;
	}
}

void TCPSocket::shutdown()
{
	this->_socketState = SocketState::NOT_HOSTING;
	#ifdef _WIN32
	closesocket(this->_socket);
	#else
	close(this->_socket);
	#endif
}

bool TCPSocket::connectTo(const std::string &ip)
{
	this->_self.sin_addr.s_addr = inet_addr(ip.c_str());

	if (connect(_socket, (struct sockaddr*)&_self, sizeof(this->_self)) < 0)
		return false;
	_state = TCPState::CONNECTED;
	return true;
}

struct sockaddr_in TCPSocket::getSockaddrIn()
{
	return _self;
}

void TCPSocket::host()
{
	this->_self.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_socket, (struct sockaddr*)&_self, sizeof(this->_self)) < 0)
		throw std::runtime_error("Binding failed");
	if (listen(this->_socket, 10) != 0)
		throw std::runtime_error("Listen failed");

	this->_socketState = SocketState::HOSTING;
}

NetUser *TCPSocket::acceptClient()
{
	if (this->_socketState != SocketState::HOSTING)
	{
		throw std::runtime_error("Can't accept you're not hosting");
	}

	#ifdef _WIN32
	SOCKET new_sock;
	#else
	int new_sock;
	#endif

	NetUser *new_user = new NetUser();

#ifdef _WIN32
	int clientlen = sizeof(*new_user->getSockaddr());
#else
	socklen_t clientlen = sizeof(*new_user->getSockaddr());
#endif

	std::memset(new_user->getSockaddr()->sin_zero, 0, 8);
	if ((new_sock = accept(_socket, (struct sockaddr*)new_user->getSockaddr(), &clientlen)) <= 0)
	{
		delete new_user;
		throw std::runtime_error("Failed to accept client");
	}

	new_user->setSocket(new_sock);

	return (new_user);
}

TCPSocket::TCPState TCPSocket::getSocketState() const
{
	return this->_state;
}
