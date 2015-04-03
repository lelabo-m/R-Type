#include "SessionManager.h"
#include "Session.h"

unsigned int Session::IDPool = 0;

Session::Session(SessionManager &manager, NetUser *user) : _netUser(user),
_sessionState(CONNECTED),
_manager(manager)
{
	Session::IDPool++;
	_sessionID = Session::IDPool;
}

Session::Session(SessionManager &manager, unsigned int id) : _netUser(0),
_sessionState(CONNECTED),
_manager(manager)
{
	_sessionID = id;
}

Session::~Session()
{
	delete _netUser;
}

#ifdef _WIN32
SOCKET Session::getSocket()
{
	return this->_netUser->getSocket();
}

#else
int Session::getSocket()
{
	return this->_netUser->getSocket();
}
#endif

std::string Session::getIP() const
{
	return this->_netUser->getIP();
}

TCPFrame *Session::getTCPReceivedFrame()
{
	std::lock_guard<std::mutex> lock(_receivedTCPQueue_mutex);

	if (_receivedTCPQueue.empty())
		return NULL;
	TCPFrame *ret = _receivedTCPQueue.front();
	_receivedTCPQueue.pop();
	return ret;
}

void Session::pushTCPReceivedFrame(TCPFrame *newTCPFrame)
{
	std::lock_guard<std::mutex> lock(_receivedTCPQueue_mutex);

	_receivedTCPQueue.push(newTCPFrame);
}

unsigned int Session::countTCPReceivedFrame() const
{
	return _receivedTCPQueue.size();
}

TCPFrame *Session::getTCPToSendFrame()
{
	std::lock_guard<std::mutex> lock(_toSendTCPQueue_mutex);

	if (_toSendTCPQueue.empty())
		return NULL;
	TCPFrame *ret = _toSendTCPQueue.front();
	_toSendTCPQueue.pop();
	return ret;
}

void Session::pushTCPToSendFrame(TCPFrame *newTCPFrame)
{
	std::lock_guard<std::mutex> lock(_toSendTCPQueue_mutex);

	_toSendTCPQueue.push(newTCPFrame);
}

void Session::notifyTCPSent(int id)
{
	std::lock_guard<std::mutex> lock(_tcpMutex);

	_tcpSent.insert(id);
}

bool Session::isTCPMessageSent(int id)
{
	std::lock_guard<std::mutex> lock(_tcpMutex);

	if (_tcpSent.count(id))
	{
		_tcpSent.erase(id);
		return true;
	}
	return false;
}

unsigned int Session::countTCPToSendFrame() const
{
	return _toSendTCPQueue.size();
}

UDPFrame *Session::getUDPReceivedFrame()
{
	std::lock_guard<std::mutex> lock(_receivedUDPQueue_mutex);

	if (_receivedUDPQueue.empty())
		return NULL;
	UDPFrame *ret = _receivedUDPQueue.front();
	_receivedUDPQueue.pop();
	return ret;
}

void Session::pushUDPReceivedFrame(UDPFrame *newUDPFrame)
{
	std::lock_guard<std::mutex> lock(_receivedUDPQueue_mutex);

	_receivedUDPQueue.push(newUDPFrame);
}

unsigned int Session::countUDPReceivedFrame() const
{
	return _receivedUDPQueue.size();
}

UDPFrame *Session::getUDPToSendFrame()
{
	std::lock_guard<std::mutex> lock(_toSendUDPQueue_mutex);

	UDPFrame *ret = _toSendUDPQueue.front();
	_toSendUDPQueue.pop();
	return ret;
}

void Session::pushUDPToSendFrame(UDPFrame *newUDPFrame)
{
	std::lock_guard<std::mutex> lock(_toSendUDPQueue_mutex);

	_toSendUDPQueue.push(newUDPFrame);
	_manager.notifyUDP(_sessionID);
}

unsigned int Session::countUDPToSendFrame() const
{
	return _toSendUDPQueue.size();
}

Session::ConnectionState Session::getSessionState() const
{
	return _sessionState;
}

void Session::setSessionState(Session::ConnectionState state)
{
	_sessionState = state;
}

unsigned int Session::getSessionID() const
{
	return this->_sessionID;
}

NetUser *Session::getNetUser()
{
	if (!_netUser)
		_netUser = new NetUser();
	return _netUser;
}

void Session::clearTCPReceivedFrame()
{
	std::lock_guard<std::mutex> lock(_receivedTCPQueue_mutex);

	while (!_receivedTCPQueue.empty())
	{
		delete this->_receivedTCPQueue.front();
		this->_receivedTCPQueue.pop();
	}
}

void Session::clearTCPToSendFrame()
{
	std::lock_guard<std::mutex> lock(_toSendTCPQueue_mutex);

	while (!_toSendTCPQueue.empty())
	{
		delete this->_toSendTCPQueue.front();
		this->_toSendTCPQueue.pop();
	}
}

void Session::clearUDPReceivedFrame()
{
	std::lock_guard<std::mutex> lock(_receivedUDPQueue_mutex);

	while (!_receivedUDPQueue.empty())
	{
		this->_receivedUDPQueue.front()->release();
		this->_receivedUDPQueue.pop();
	}
}

void Session::clearUDPToSendFrame()
{
	std::lock_guard<std::mutex> lock(_toSendUDPQueue_mutex);

	while (!_toSendUDPQueue.empty())
	{
		this->_toSendUDPQueue.front()->release();
		this->_toSendUDPQueue.pop();
	}
}