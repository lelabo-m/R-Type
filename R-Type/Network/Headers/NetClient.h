#ifndef _NETCLIENT_H_
# define _NETCLIENT_H_

#include "NetClientManager.h"

class NetClient
{
public:
	NetClient();
	~NetClient();
	void connect(const std::string &ip, unsigned short port, bool noUDP = false);
	void disconnect();
	NetClientManager::ConnectionState getConnectionState() const;
	std::shared_ptr<Session> getOwnSession();
	unsigned int getOwnSessionID() const;
	UDPFramePool	&getFramePool();

protected:
	NetClientManager	_clientManager;
	UDPFramePool		_pool;
};

#endif