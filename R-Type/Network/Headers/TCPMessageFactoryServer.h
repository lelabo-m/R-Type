#ifndef _TCPMESSAGEFACTORYSERVER_H_
# define _TCPMESSAGEFACTORYSERVER_H_

#include "TCPMessageFactory.h"

class TCPMessageFactoryServer : public TCPMessageFactory
{
public:
	static TCPMessageFactory *getInstance();
	~TCPMessageFactoryServer();
protected:
	virtual void incrementID();
	TCPMessageFactoryServer();
};

#endif