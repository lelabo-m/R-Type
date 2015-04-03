#include "Lobby.h"


Lobby::Lobby(const std::string &gamename,
			const std::string &mapname,
			const std::string &leadername) :
			_isConfirmed(false),
			_gameName(gamename),
			_mapName(mapname),
			_owner(leadername)
{
	_players.push_back(leadername);
}

Lobby::~Lobby()
{
}

void Lobby::refreshPlayers(std::list<std::string> &players)
{
	_players.clear();
	_players = players;
}

const std::string &Lobby::getGameName() const
{
	return _gameName;
}

const std::string &Lobby::getMapName() const
{
	return _mapName;
}

void Lobby::setMapName(const std::string &name)
{
	_mapName = name;
}

const std::list<std::string> &Lobby::getPlayers() const
{
	return _players;
}

const std::string &Lobby::getOwner() const
{
	return _owner;
}

bool Lobby::isConfirmed() const
{
	return _isConfirmed;
}

void Lobby::confirmedByTheServer()
{
	_isConfirmed = true;
}