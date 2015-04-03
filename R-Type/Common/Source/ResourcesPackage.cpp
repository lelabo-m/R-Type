#include "ResourcesPackage.h"

ResourcesPackage::ResourcesPackage(FileToSend map,
	std::list<FileToSend> &files,
	std::list<Animation> &animations,
	TCPMessageFactory *fac,
	int animCode) :
	_animationCode(animCode),
	_factory(fac),
	_mapToSend(map),
	_filesToSend(files),
	_animationsToSend(animations)
{
}

ResourcesPackage::~ResourcesPackage()
{
}

void ResourcesPackage::sendToPlayers(std::list<std::shared_ptr<Session> > &sessions)
{
	for (auto it = sessions.begin(); it != sessions.end(); it++)
	{
		_listDest.push_back(std::pair<std::shared_ptr<Session>, std::queue<int>>(*it, std::queue<int>()));
		fillReceiver(_listDest.back());
	}
}

void ResourcesPackage::sendToPlayer(std::shared_ptr<Session> &session)
{
	_listDest.push_back(std::pair<std::shared_ptr<Session>, std::queue<int>>(session, std::queue<int>()));
	fillReceiver(_listDest.back());
}

void ResourcesPackage::fillReceiver(std::pair<std::shared_ptr<Session>, std::queue<int>> &receiver)
{
	if (_mapToSend.remote.size())
		receiver.second.push(_factory->SendFile(receiver.first, _mapToSend.local, _mapToSend.remote));

	for (auto it = _filesToSend.begin(); it != _filesToSend.end(); it++)
	{
		receiver.second.push(_factory->SendFile(receiver.first, it->local, it->remote));
	}

	for (auto it = _animationsToSend.begin(); it != _animationsToSend.end(); it++)
	{
		receiver.second.push(_factory->SendMsg(receiver.first, Animation::Serialize(*it), _animationCode));
	}
}

bool ResourcesPackage::hasPlayerLoaded(std::shared_ptr<Session> &session)
{
	for (auto it = _listDest.begin(); it != _listDest.end(); it++)
	{
		if (it->first->getSessionID() == session->getSessionID())
		{
			if (it->second.size() == 0)
				return true;
			while (session->isTCPMessageSent(it->second.front()))
			{
				it->second.pop();
				if (it->second.size() == 0)
					return true;
			}
			break;
		}
	}
	return false;
}