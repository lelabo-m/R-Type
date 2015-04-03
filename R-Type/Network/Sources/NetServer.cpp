#include "NetServer.h"


NetServer::NetServer()
{
}

NetServer::~NetServer()
{
}

void NetServer::makeOnline(unsigned short port)
{
	try
	{
		_serverManager.makeOnline(port);
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
}

void NetServer::makeOffline()
{
	try
	{
		_serverManager.makeOffline();
	}
	catch (std::runtime_error &e)
	{
		throw e;
	}
}

std::list<std::shared_ptr<Session>> NetServer::getFreshSessions()
{
	return _serverManager.getFreshSessions();
}

unsigned int NetServer::countFreshSessions() const
{
	return _serverManager.countFreshSessions();
}