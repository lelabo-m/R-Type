#ifndef _TCPMESSAGEFACTORY_H_
# define _TCPMESSAGEFACTORY_H_

#include "Session.h"
#include "Values.h"
#include "FileManager.h"

class TCPMessageFactory
{
public:
	int SendMsg(std::shared_ptr<Session>, const std::string &msg, int procedureCode);
	int SendFile(std::shared_ptr<Session>, const std::string &file_path, const std::string &remote_path, int = Protocole::Server::SEND_FILE);
	virtual ~TCPMessageFactory() { };

protected:
	virtual void incrementID() = 0;
	TCPMessageFactory();

protected:
	static TCPMessageFactory *_instance;
	int _currentID;
};

#endif