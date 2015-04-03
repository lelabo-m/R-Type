#ifndef _UDPMANAGERCLIENT_H_
# define _UDPMANAGERCLIENT_H_

#include "UDPManager.h"
class UDPManagerClient :
	public UDPManager
{
public:
	UDPManagerClient(SessionManager &, const std::string &, unsigned short port, bool noUDP = false);
	~UDPManagerClient();
protected:
};

#endif