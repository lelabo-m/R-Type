#ifndef _TCPMESSAGEFACTORYCLIENT_H_
# define _TCPMESSAGEFACTORYCLIENT_H_

#include "TCPMessageFactory.h"

class TCPMessageFactoryClient : public TCPMessageFactory
{
public:
	static TCPMessageFactory *getInstance();
	~TCPMessageFactoryClient();
protected:
	virtual void incrementID();
	TCPMessageFactoryClient();
};

#endif