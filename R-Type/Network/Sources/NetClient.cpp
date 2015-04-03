#include "NetClient.h"

NetClient::NetClient()
{
}


NetClient::~NetClient()
{
}

void NetClient::connect(const std::string &ip, unsigned short port, bool noUDP)
{
	try
	{
		_clientManager.connect(ip, port, noUDP);
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
}

void NetClient::disconnect()
{
	try
	{
		_clientManager.disconnect();
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
}

NetClientManager::ConnectionState NetClient::getConnectionState() const
{
	return _clientManager.getConnectionState();
}

std::shared_ptr<Session> NetClient::getOwnSession()
{
	try
	{
		return _clientManager.getOwnSession();
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
}

unsigned int NetClient::getOwnSessionID() const
{
	return _clientManager.getOwnSessionID();
}

UDPFramePool	&NetClient::getFramePool()
{
	return this->_pool;
}