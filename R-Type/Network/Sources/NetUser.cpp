#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"

NetUser::NetUser()
{
}

NetUser::~NetUser()
{
}


std::string NetUser::getIP() const
{
	return (inet_ntoa(this->_sockaddr.sin_addr));
}

struct sockaddr_in *NetUser::getSockaddr()
{
	return &(this->_sockaddr);
}

#ifdef _WIN32
SOCKET NetUser::getSocket()
{
	return this->_socket;
}

void NetUser::setSocket(SOCKET socket)
{
	this->_socket = socket;
}

#else
int NetUser::getSocket()
{
	return this->_socket;
}

void NetUser::setSocket(int socket)
{
	this->_socket = socket;
}
#endif