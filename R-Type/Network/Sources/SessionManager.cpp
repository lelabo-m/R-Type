#include "SessionManager.h"
#include "UDPManager.h"

SessionManager::SessionManager()
{
	_sessions.clear();
	this->eraseDisconnectedList();
	while (!_udpSlot.empty())
		_udpSlot.pop();
}

SessionManager::~SessionManager()
{
	_sessions.clear();
	this->eraseDisconnectedList();
	while (!_udpSlot.empty())
		_udpSlot.pop();
}

void SessionManager::createNewSession(NetUser *new_user)
{
	std::lock_guard<std::mutex> lock(_sessionMutex);

	unsigned int id = Session::IDPool + 1;
	_sessions[id] = std::make_shared<Session>(*this, new_user);
	_freshSessions.push_back(_sessions[Session::IDPool]);
}

void SessionManager::createNewSession(unsigned int id)
{
	std::lock_guard<std::mutex> lock(_sessionMutex);

	_sessions[id] = std::make_shared<Session>(*this, id);
}

bool SessionManager::sessionExist(unsigned int id) const
{
	return (_sessions.count(id)) ? true : false;
}

std::shared_ptr<Session> SessionManager::getSession(unsigned int id)
{
	return _sessions[id];
}

Session *SessionManager::getSessionPtr(unsigned int id)
{
	Session *session = NULL;

	try
	{
		if (_sessions.count(id) == 0)
			return NULL;
		session = _sessions[id].get();
	}
	catch (std::exception &e)
	{
		(void)e;
		session = 0;
	}
	return session;
}

std::list<std::shared_ptr<Session>> SessionManager::getAllSession()
{
	std::lock_guard<std::mutex> lock(_sessionMutex);

	std::list<std::shared_ptr<Session>> socket_list;

	auto it = _sessions.begin();
	while (it != _sessions.end())
	{
		Session *current_session = (*it).second.get();
		if (current_session)
		{
			if (current_session->getSessionState() == Session::CONNECTED)
			{
				socket_list.push_back((*it).second);
				it++;
			}
			else
			{
				current_session->UDPSending.lock();
				current_session->UDPReceiving.lock();
				_disconnectedSessions.push_back((*it).second);
				it = _sessions.erase(it);
			}
		}
		else
		{
			it = _sessions.erase(it);
		}
	}
	return socket_list;
}

std::list<std::shared_ptr<Session>> SessionManager::getFreshSessions()
{
	std::list<std::shared_ptr<Session>> _newList;
	std::lock_guard<std::mutex> lock(_sessionMutex);

	_newList = _freshSessions;
	_freshSessions.clear();
	return _newList;
}

unsigned int SessionManager::countFreshSessions() const
{
	return _freshSessions.size();
}

void SessionManager::eraseDisconnectedList()
{
	for (auto it = _disconnectedSessions.begin(); it != _disconnectedSessions.end(); it++)
	{
		it->get()->UDPReceiving.unlock();
		it->get()->UDPSending.unlock();
	}
	_disconnectedSessions.clear();
}

unsigned int SessionManager::getSessionCount() const
{
	return _sessions.size();
}

void SessionManager::notifyUDP(unsigned int id)
{
	_udpMutex.lock();
	_udpSlot.push(id);
//	if (_udpSlot.size() == 1)
		UDPManager::NotifyUDPSender.notify_one();
	_udpMutex.unlock();
}

bool SessionManager::UDPSlotEmpty() const
{
	return _udpSlot.empty();
}

unsigned int SessionManager::getNextNotifiedSessionUDP()
{
	_udpMutex.lock();
	unsigned int id = _udpSlot.front();
	_udpSlot.pop();
	_udpMutex.unlock();
	return id;
}
