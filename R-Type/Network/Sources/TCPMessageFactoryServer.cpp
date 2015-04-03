#include "TCPMessageFactoryServer.h"

TCPMessageFactoryServer::TCPMessageFactoryServer()
{
	_currentID = 1;
}

TCPMessageFactoryServer::~TCPMessageFactoryServer()
{
}

TCPMessageFactory *TCPMessageFactoryServer::getInstance()
{
	if (_instance)
		return _instance;
	_instance = new TCPMessageFactoryServer();
	return _instance;
}

void TCPMessageFactoryServer::incrementID()
{
	this->_currentID++;
	if (_currentID > 2000000)
		_currentID = 1;
}