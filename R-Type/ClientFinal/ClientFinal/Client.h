#ifndef _CLIENT_H_
# define _CLIENT_H_

#include <map>
#include <functional>
#include "NetClient.h"
#include "Values.h"
#include "SFMLanimatedSprite.h"
#include "GraphicalElem.h"
#include "GameSession.h"
#include "Lobby.h"
#include "TCPMessageFactoryClient.h"

class Client
{
public:
	static Client *getInstance()
	{
		if (_client)
			return _client;
		_client = new Client();
		return _client;
	}
	NetClient &getNetClient();
	void handleNetwork();
	void askLobbyInformation();
	void askAvailableMaps();
	void askAvailableGames();
	void askToLaunch();
	void askJoinGame(const std::string &gameName, const std::string &ownNickName);
	void askCreateGame(const std::string &gameName, const std::string &ownNickName);
	void askLeaveLobby();
	~Client();

public:
	std::list<std::string> AvailableMaps;
	std::list<GameSession> AvailableGames;
	Lobby *CurrentLobby;

protected:
	Client();
	void handle_list_maps(TCPFrame*);
	void handle_list_games(TCPFrame*);
	void handle_lobby_information(TCPFrame*);
	void handle_kicked_lobby(TCPFrame*);
	void setNetworkHandlers();
	std::string popToken(std::string &, const std::string &);
private:
	static Client *_client;
protected:
	NetClient				_netClient;

public:
	SFMLanimationManager&	AnimManager();
protected:
	TCPMessageFactory *_msgFactory;
	std::map<Protocole::Server::Procedure, std::function<void(TCPFrame*)>> _netFunctions;
	SFMLanimationManager		_animManager;
	std::list<GraphicalElem>	_example;
};

#endif