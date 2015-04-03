#ifndef _NETSERVERMANAGER_H_
# define _NETSERVERMANAGER_H_

#include "TCPManagerServer.h"
#include "UDPManager.h"
#include "SessionManager.h"

class NetServerManager
{
public:
	NetServerManager();
	~NetServerManager();
	void makeOnline(unsigned short port);
	void makeOffline();
	std::list<std::shared_ptr<Session>> getFreshSessions();
	unsigned int countFreshSessions() const;

private:
	bool _on;
	SessionManager *_sessionManager;
	TCPManager *_tcpManager;
	UDPManager *_udpManager;
};

#endif