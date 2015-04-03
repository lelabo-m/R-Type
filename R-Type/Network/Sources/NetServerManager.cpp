#include "NetServerManager.h"


NetServerManager::NetServerManager() : _on(false), _sessionManager(0), _tcpManager(0), _udpManager(0)
{
}

NetServerManager::~NetServerManager()
{
	if (_on == true)
	{
		makeOffline();
	}
}

void NetServerManager::makeOnline(unsigned short port)
{
	if (_on == true)
		return;

	_sessionManager = new SessionManager();
	_tcpManager = new TCPManagerServer(*_sessionManager, port);
	try
	{
		_tcpManager->host();
	}
	catch (std::runtime_error &e)
	{
		delete _sessionManager;
		delete _tcpManager;
		throw e;
	}
	_udpManager = new UDPManager(*_sessionManager, port);
	_on = true;
}

void NetServerManager::makeOffline()
{
	if (_on == false)
		return;

	_tcpManager->shutdown();
	_udpManager->shutdown();
	delete _tcpManager;
	delete _udpManager;
	delete _sessionManager;

	_on = false;
}

std::list<std::shared_ptr<Session>> NetServerManager::getFreshSessions()
{
	if (_on == false)
		throw std::runtime_error("Can't access sessions, server is offline");
	return _sessionManager->getFreshSessions();
}

unsigned int NetServerManager::countFreshSessions() const
{
	if (_on == false)
		throw std::runtime_error("Can't access sessions, server is offline");
	return _sessionManager->countFreshSessions();
}