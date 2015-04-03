#include "Client.h"

Client *Client::_client = nullptr;

Client::Client() : CurrentLobby(nullptr), _msgFactory(TCPMessageFactoryClient::getInstance())
{
	setNetworkHandlers();
	AvailableMaps.clear();
	AvailableGames.clear();
}

Client::~Client()
{
}

NetClient &Client::getNetClient()
{
	return _netClient;
}

SFMLanimationManager&	Client::AnimManager()
{
	return _animManager;
}

std::string Client::popToken(std::string &str, const std::string &delim)
{
	size_t pos;
	while ((pos = str.find(delim)) != std::string::npos)
	{
		std::string token = str.substr(0, pos);
		str.erase(0, pos + delim.length());
		return token;
	}
	return "";
}

void Client::setNetworkHandlers()
{
	_netFunctions[Protocole::Server::MAP_LIST] = std::bind(&Client::handle_list_maps, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_LIST] = std::bind(&Client::handle_list_games, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::LOBBY_INFORMATION] = std::bind(&Client::handle_lobby_information, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::KICK_FROM_LOBBY] = std::bind(&Client::handle_kicked_lobby, this, std::placeholders::_1);
}

void Client::handleNetwork()
{
	if (_netClient.getConnectionState() == NetClientManager::DISCONNECTED)
		throw std::runtime_error("DISCONNECTED");
	if (_netClient.getOwnSessionID() == 0)
		return;
	std::shared_ptr<Session> session = _netClient.getOwnSession();
	unsigned int count = session.get()->countTCPReceivedFrame();

	for (; count > 0; count--)
	{
		if (TCPFrame *frame = session.get()->getTCPReceivedFrame())
		{
			Protocole::Server::Procedure proc = static_cast<Protocole::Server::Procedure>(frame->getHeader().getProcedureCode());

			if (_netFunctions.count(proc))
			{
				_netFunctions[proc](frame);
			}
			delete frame;
		}
	}
}

void Client::handle_list_maps(TCPFrame *frame)
{
	unsigned int size = frame->getHeader().getDataLength();
	std::string data;
	data.insert(0, &frame->getData()[TCP_HEADER_LENGTH], size);

	std::string delimiter = ",";
	std::string token;
	while ((token = popToken(data, delimiter)) != "")
	{
		if (std::find(std::begin(AvailableMaps), std::end(AvailableMaps), token) == std::end(AvailableMaps))
		{
			AvailableMaps.push_back(token);
		}
	}
}

void Client::handle_list_games(TCPFrame *frame)
{
	unsigned int size = frame->getHeader().getDataLength();
	std::string data;
	data.insert(0, &frame->getData()[TCP_HEADER_LENGTH], size);

	std::string delimiter1 = ",";
	std::string delimiter2 = ";";
	std::string token;
	while ((token = popToken(data, delimiter1)) != "")
	{
		std::string gamename = token;
		std::string mapname = popToken(data, delimiter1);
		if (mapname == "")
			return;
		std::string nbPlayer = popToken(data, delimiter2);
		if (nbPlayer == "")
			return;
		AvailableGames.push_back(GameSession(gamename, mapname, std::stoul(nbPlayer)));
	}
}

void Client::handle_lobby_information(TCPFrame *frame)
{
	unsigned int size = frame->getHeader().getDataLength();
	std::string data;
	data.insert(0, &frame->getData()[TCP_HEADER_LENGTH], size);

	std::string delimiter = ",";
	std::string gamename = popToken(data, delimiter);
	if (gamename == "")
		return;
	std::string mapname = popToken(data, delimiter);
	if (mapname == "")
		return;
	std::string playerOwner = popToken(data, delimiter);
	if (playerOwner == "")
		return;

	std::string newPlayer;
	std::list<std::string> players;
	players.push_back(playerOwner);
	while ((newPlayer = popToken(data, delimiter)) != "")
		players.push_back(newPlayer);

	if (CurrentLobby == nullptr)
		CurrentLobby = new Lobby(gamename, mapname, playerOwner);
	else
	{
		if (CurrentLobby->getGameName() != gamename)
		{
			delete CurrentLobby;
			CurrentLobby = new Lobby(gamename, mapname, playerOwner);
		}
	}
	CurrentLobby->refreshPlayers(players);
}

void Client::handle_kicked_lobby(TCPFrame *)
{
	if (CurrentLobby)
		delete CurrentLobby;
	CurrentLobby = nullptr;
	// Call changement de menu, quitter le lobby
}

void Client::askLobbyInformation()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LOBBY_INFORMATION);
}

void Client::askAvailableMaps()
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::MAP_LIST);
}

void Client::askAvailableGames()
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::GAME_LIST);
}

void Client::askToLaunch()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LAUNCH_GAME);
}

void Client::askJoinGame(const std::string &gameName, const std::string &ownNickName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), gameName + ";" + ownNickName + ";", Protocole::Client::JOIN_GAME);
}

void Client::askCreateGame(const std::string &gameName, const std::string &ownNickName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), gameName + ";" + ownNickName + ";", Protocole::Client::CREATE_GAME);
}

void Client::askLeaveLobby()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LEAVE_LOBBY);
	if (CurrentLobby)
		delete CurrentLobby;
	CurrentLobby = nullptr;
}