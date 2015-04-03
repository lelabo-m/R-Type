#include "TCPMessageFactoryClient.h"


TCPMessageFactoryClient::TCPMessageFactoryClient()
{
	_currentID = -1;
}

TCPMessageFactoryClient::~TCPMessageFactoryClient()
{
}

TCPMessageFactory *TCPMessageFactoryClient::getInstance()
{
	if (_instance)
		return _instance;
	_instance = new TCPMessageFactoryClient();
	return _instance;
}

void TCPMessageFactoryClient::incrementID()
{
	this->_currentID--;
	if (_currentID < -2000000)
		_currentID = -1;
}