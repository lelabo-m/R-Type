#ifndef _SESSIONMANAGER_H_
# define _SESSIONMANAGER_H_

#include <unordered_map>
#include <memory>
#include <queue>
#include <list>
#include "Session.h"

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	void createNewSession(NetUser*);
	void createNewSession(unsigned int id);
	bool sessionExist(unsigned int) const;
	std::shared_ptr<Session> getSession(unsigned int);
	Session *getSessionPtr(unsigned int);
	unsigned int getSessionCount() const;
	std::list<std::shared_ptr<Session>> getAllSession();
	std::list<std::shared_ptr<Session>> getFreshSessions();
	unsigned int countFreshSessions() const;
	void eraseDisconnectedList();
	void notifyUDP(unsigned int);

	bool UDPSlotEmpty() const;
	unsigned int getNextNotifiedSessionUDP();

public:
	std::mutex _sessionMutex;

protected:
	std::unordered_map<unsigned int, std::shared_ptr<Session>> _sessions;
	std::list<std::shared_ptr<Session>> _freshSessions;
	std::list<std::shared_ptr<Session>> _disconnectedSessions;

protected:
	std::mutex _udpMutex;
	std::queue<unsigned int> _udpSlot;
};

#endif