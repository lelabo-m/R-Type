#include "NetClientManager.h"

NetClientManager::NetClientManager() : _ownSessionID(0), _status(DISCONNECTED), _sessionManager(0), _tcpManager(0), _udpManager(0)
{
}

NetClientManager::~NetClientManager()
{
}

void NetClientManager::connect(const std::string &ip, unsigned short port, bool noUDP)
{
	if (_status != DISCONNECTED)
		throw std::runtime_error("Can't try to connect when you're connected or trying to be");
	_status = CONNECTING;
	_sessionManager = new SessionManager();
	_tcpManager = new TCPManagerClient(*_sessionManager, port, *this);
	try
	{
		_tcpManager->connect(ip);
	}
	catch (std::runtime_error &e)
	{
		delete _sessionManager;
		delete _tcpManager;
		_status = DISCONNECTED;
		throw e;
	}
	_udpManager = new UDPManagerClient(*_sessionManager, ip, port, noUDP);
	_status = CONNECTED;
}

void NetClientManager::disconnect()
{
	std::lock_guard<std::mutex> lock(_netClientManagerMutex);

	_tcpManager->shutdown();
	_udpManager->shutdown();
	delete _tcpManager;
	delete _sessionManager;
	delete _udpManager;
	_ownSessionID = 0;

	_status = DISCONNECTED;
}

std::shared_ptr<Session> NetClientManager::getOwnSession()
{
	if (_status != CONNECTED)
		throw std::runtime_error("Can't get session id when you're not connected");
	if (_ownSessionID == 0)
		throw std::runtime_error("Player session id still not received");
	return _sessionManager->getSession(_ownSessionID);
}

NetClientManager::ConnectionState NetClientManager::getConnectionState() const
{
	return _status;
}

void NetClientManager::setConnectionState(ConnectionState state)
{
	std::lock_guard<std::mutex> lock(_netClientManagerMutex);
	_status = state;
}

unsigned int NetClientManager::getOwnSessionID() const
{
	return _ownSessionID;
}

void NetClientManager::setOwnSessionID(unsigned int id)
{
	std::lock_guard<std::mutex> lock(_netClientManagerMutex);
	_ownSessionID = id;
}