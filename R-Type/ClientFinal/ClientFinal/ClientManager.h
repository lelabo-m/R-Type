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
#include "MapDownloader.h"
#include "Game.h"

# define MAPDIR "maps"

class ClientManager
{
public:
	static ClientManager *getInstance()
	{
		if (_client)
			return _client;
		_client = new ClientManager();
		return _client;
	}
	NetClient &getNetClient();
	void handleNetwork();
	void askLobbyInformation();
	void askAvailableMaps();
	void askAvailableGames();
	void askToLaunch();
	void askJoinGame(const std::string &gameName, const std::string &ownNickName);
	void askCreateGame(const std::string &gameName, const std::string &ownNickName, bool isPrivate = false);
	void askLeaveLobby();
	void askMap(const std::string &);
	void askSendMap(const std::string &);
	void askChangeMap(const std::string &);
	void askSendLobbyMessage(const std::string &);
	void askMapScore(const std::string &);
	~ClientManager();

protected:
	std::chrono::high_resolution_clock::time_point timepoint;

public:
	std::list<std::string> AvailableMaps;
	std::list<GameSession> AvailableGames;
	MapDownloader *CurrentMapDownload;
	Lobby *CurrentLobby;
	Game *CurrentGame;
	std::list<std::pair<std::string, std::string>> Scores;


protected:
	ClientManager();
	void handle_list_maps(TCPFrame*);
	void handle_list_games(TCPFrame*);
	void handle_lobby_information(TCPFrame*);
	void handle_kicked_lobby(TCPFrame*);
	void handle_game_creation_failed(TCPFrame*);
	void handle_game_creation_succeed(TCPFrame*);
	void handle_game_join_failed(TCPFrame*);
	void handle_game_join_succeed(TCPFrame*);
	void handle_received_file(TCPFrame*);
	void handle_animation(TCPFrame*);
	void handle_ask_map_succeed(TCPFrame*);
	void handle_ask_map_failed(TCPFrame*);
	void handle_map_download_finished(TCPFrame*);
	void handle_game_loading(TCPFrame *);
	void handle_game_loading_finished(TCPFrame *);
	void handle_game_synchronization(TCPFrame *);
	void handle_game_synchronization_finished(TCPFrame *);
	void handle_game_started(TCPFrame *);
	void handle_new_lobby_message(TCPFrame *);
	void handle_list_map_score(TCPFrame *);
	void handle_ask_score(TCPFrame *);
	void handle_game_finished(TCPFrame *);

	void setNetworkHandlers();
	std::string popToken(std::string &, const std::string &);
private:
	static ClientManager *_client;
protected:
	NetClient				_netClient;

public:
	SFMLanimationManager&	AnimManager();
	const std::list<GraphicalElem>	&getGraphicalElements() const;
	const std::chrono::high_resolution_clock::time_point &getTimePoint() const;
	void	clearGraphicalElements();

protected:
	TCPMessageFactory *_msgFactory;
	std::map<Protocole::Server::Procedure, std::function<void(TCPFrame*)>> _netFunctions;
	SFMLanimationManager		_animManager;
	std::list<GraphicalElem>	_example;
};

#endif