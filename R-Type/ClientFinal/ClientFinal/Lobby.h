#ifndef _LOBBY_H_
# define _LOBBY_H_

#include <list>
#include <string>

class Lobby
{
public:
	Lobby(const std::string &gamename,
		const std::string &mapname,
		const std::string &leadername);
	void refreshPlayers(std::list<std::string> &players);
	const std::string &getGameName() const;
	const std::string &getMapName() const;
	void setMapName(const std::string &);
	const std::list<std::string> &getPlayers() const;
	const std::string &getOwner() const;
	bool isConfirmed() const;
	void confirmedByTheServer();

	~Lobby();

public:
	std::list<std::string> Messages;

protected:
	bool _isConfirmed;
	std::string _gameName;
	std::string _mapName;
	std::list<std::string> _players;
	std::string _owner;
};

#endif