#ifndef _TCPMANAGERSERVER_H_
# define _TCPMANAGERSERVER_H_

#include "TCPManager.h"

class TCPManagerServer : public TCPManager
{
public:
	explicit TCPManagerServer(SessionManager &, unsigned short port);
	~TCPManagerServer();

protected:
	virtual void otherNotification();
	virtual void ownReadNotified();
	void sendSessionID(unsigned int id);

protected:
	int _msgIDPool;
};

#endif
