#ifndef _TCPMANAGERCLIENT_H_
# define _TCPMANAGERCLIENT_H_

#include "TCPManager.h"

class NetClientManager;

class TCPManagerClient : public TCPManager
{
public:
	TCPManagerClient(SessionManager &, unsigned short port, NetClientManager &);
	~TCPManagerClient();

protected:
	NetClientManager &_clientManager;

protected:
	virtual void otherNotification();
	virtual void ownReadNotified();
};

#endif