#ifndef _TCPMANAGER_H_
# define _TCPMANAGER_H_

#include <memory>
#include <thread>
#include "TCPSocket.h"
#include "SessionManager.h"

class TCPManager
{
	enum TCPType
	{
		CLIENT,
		SERVER
	};

public:
	static const unsigned int CYCLE_TIME = 50000000;

public:
	TCPManager(SessionManager &, unsigned short port);
	virtual ~TCPManager();

	void connect(const std::string &ip);
	void host();
	void shutdown();

protected:
	virtual void otherNotification() = 0;
	virtual void ownReadNotified() = 0;
	void thread_selectLoop();
	void serverNotifiedSocket();
	void clientNotifiedSocket();
	void setFS();
	void checkClients();
	void clientWritePossible();
	void refreshSessions();
	bool clock(struct timeval &tv, std::chrono::high_resolution_clock::time_point &start);
protected:
	bool _running;
	int _maxSock;
	std::thread _tcpThread;
	std::list<std::shared_ptr<Session>> _sockets;
	TCPType _type;
	fd_set _rdfs;
	fd_set _wrfs;
	unsigned short _port;
	TCPSocket _ownSock;
	SessionManager &_sessionManager;
};

#endif