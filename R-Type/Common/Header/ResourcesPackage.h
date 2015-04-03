#ifndef _RESOURCESPACKAGE_H_
# define _RESOURCESPACKAGE_H_

#include <queue>
#include "TCPMessageFactoryServer.h"
#include "TCPMessageFactoryClient.h"
#include "ObjectRepresentation.hpp"

class ResourcesPackage
{
public:
	virtual ~ResourcesPackage();
	void sendToPlayers(std::list<std::shared_ptr<Session>> &);
	void sendToPlayer(std::shared_ptr<Session> &);
	bool hasPlayerLoaded(std::shared_ptr<Session> &session);

protected:
	ResourcesPackage(FileToSend, std::list<FileToSend> &, std::list<Animation> &, TCPMessageFactory *, int);
	void fillReceiver(std::pair<std::shared_ptr<Session>, std::queue<int>> &);

protected:
	const int _animationCode;
	TCPMessageFactory *_factory;
	std::list<std::pair<std::shared_ptr<Session>, std::queue<int>>> _listDest;
	FileToSend _mapToSend;
	std::list<FileToSend> _filesToSend;
	std::list<Animation> _animationsToSend;
};

#endif
