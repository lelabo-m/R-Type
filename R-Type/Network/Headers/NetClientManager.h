#ifndef _NETCLIENTMANAGER_H_
# define _NETCLIENTMANAGER_H_

#include "TCPManagerClient.h"
#include "UDPManagerClient.h"
#include "SessionManager.h"

class NetClientManager
{
public:
	enum ConnectionState
	{
		DISCONNECTED,
		CONNECTING,
		CONNECTED
	};

public:
	NetClientManager();
	~NetClientManager();
	void connect(const std::string &ip, unsigned short port, bool noUDP = false);
	void disconnect();
	ConnectionState getConnectionState() const;
	void setConnectionState(ConnectionState);
	std::shared_ptr<Session> getOwnSession();
	unsigned int getOwnSessionID() const;
	void setOwnSessionID(unsigned int);

protected:
	std::mutex _netClientManagerMutex;
	unsigned int _ownSessionID;
	ConnectionState _status;
	SessionManager *_sessionManager;
	TCPManager *_tcpManager;
	UDPManager *_udpManager;
};

#endif
