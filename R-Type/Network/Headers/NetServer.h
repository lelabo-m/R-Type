#ifndef _NETSERVER_H_
# define _NETSERVER_H_

#include "NetServerManager.h"

class NetServer
{
public:
	NetServer();
	~NetServer();
	void makeOnline(unsigned short port);
	void makeOffline();
	std::list<std::shared_ptr<Session>> getFreshSessions();
	unsigned int countFreshSessions() const;
protected:
	NetServerManager _serverManager;
};

#endif