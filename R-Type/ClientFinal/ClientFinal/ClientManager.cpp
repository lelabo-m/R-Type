#include "ClientManager.h"
#include "ResourcesPackageClient.h"

ClientManager *ClientManager::_client = nullptr;

ClientManager::ClientManager() : CurrentMapDownload(nullptr), CurrentLobby(nullptr), CurrentGame(nullptr), _msgFactory(TCPMessageFactoryClient::getInstance())
{
	setNetworkHandlers();
	AvailableMaps.clear();
	AvailableGames.clear();
}

ClientManager::~ClientManager()
{
}

NetClient &ClientManager::getNetClient()
{
	return _netClient;
}

SFMLanimationManager&	ClientManager::AnimManager()
{
	return _animManager;
}

std::string ClientManager::popToken(std::string &str, const std::string &delim)
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

void ClientManager::setNetworkHandlers()
{
	_netFunctions[Protocole::Server::MAP_LIST] = std::bind(&ClientManager::handle_list_maps, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_LIST] = std::bind(&ClientManager::handle_list_games, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::LOBBY_INFORMATION] = std::bind(&ClientManager::handle_lobby_information, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::KICK_FROM_LOBBY] = std::bind(&ClientManager::handle_kicked_lobby, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_CREATION_FAILED] = std::bind(&ClientManager::handle_game_creation_failed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_CREATION_SUCCESS] = std::bind(&ClientManager::handle_game_creation_succeed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::JOIN_GAME_FAILED] = std::bind(&ClientManager::handle_game_join_failed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::JOIN_GAME_SUCCESS] = std::bind(&ClientManager::handle_game_join_succeed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::SEND_FILE] = std::bind(&ClientManager::handle_received_file, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::SEND_ANIMATION] = std::bind(&ClientManager::handle_animation, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::ASK_MAP_FAILED] = std::bind(&ClientManager::handle_ask_map_failed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::ASK_MAP_SUCCEED] = std::bind(&ClientManager::handle_ask_map_succeed, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::MAP_DOWNLOAD_FINISH] = std::bind(&ClientManager::handle_map_download_finished, this, std::placeholders::_1);

	_netFunctions[Protocole::Server::GAME_LOADING] = std::bind(&ClientManager::handle_game_loading, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_LOADING_FINISHED] = std::bind(&ClientManager::handle_game_loading_finished, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_SYNCHRONIZATION] = std::bind(&ClientManager::handle_game_synchronization, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_SYNCHRONIZATION_FINISHED] = std::bind(&ClientManager::handle_game_synchronization_finished, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_STARTED] = std::bind(&ClientManager::handle_game_started, this, std::placeholders::_1);

	_netFunctions[Protocole::Server::NEW_LOBBY_MESSAGE] = std::bind(&ClientManager::handle_new_lobby_message, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::ASK_MAP_SCORES] = std::bind(&ClientManager::handle_ask_score, this, std::placeholders::_1);
	_netFunctions[Protocole::Server::GAME_FINISHED] = std::bind(&ClientManager::handle_game_finished, this, std::placeholders::_1);
}

void ClientManager::handleNetwork()
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

void ClientManager::handle_list_maps(TCPFrame *frame)
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

void ClientManager::handle_list_games(TCPFrame *frame)
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

void ClientManager::handle_lobby_information(TCPFrame *frame)
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
	mapname = FileSystem::FileShortName(mapname);

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
		else
		{
			CurrentLobby->setMapName(mapname);
		}
	}
	CurrentLobby->refreshPlayers(players);
}

void ClientManager::handle_game_creation_failed(TCPFrame*)
{
	std::cout << "Game creation failed" << std::endl;
	delete CurrentLobby;
	CurrentLobby = nullptr;
}

void ClientManager::handle_game_creation_succeed(TCPFrame*)
{
	if (CurrentLobby != nullptr)
		CurrentLobby->confirmedByTheServer();
}

void ClientManager::handle_game_join_failed(TCPFrame*)
{
	delete CurrentLobby;
	CurrentLobby = nullptr;
}

void ClientManager::handle_game_join_succeed(TCPFrame*)
{
	if (CurrentLobby != nullptr)
		CurrentLobby->confirmedByTheServer();
}

void ClientManager::handle_kicked_lobby(TCPFrame *)
{
	if (CurrentLobby)
		delete CurrentLobby;
	CurrentLobby = nullptr;
	//Call changement de menu, quitter le lobby
}

void ClientManager::handle_received_file(TCPFrame *frame)
{

	std::string file_path = &frame->getData()[TCP_HEADER_LENGTH];
	std::string data;

	data.insert(0, &frame->getData()[TCP_HEADER_LENGTH + file_path.size() + 1],
		frame->getHeader().getDataLength() - file_path.size() - 1);
	file_path.insert(0, std::string(MAPDIR + std::string(SLASHSEP)));
	FileManager::writeFile(file_path, data);
}

void ClientManager::handle_animation(TCPFrame *frame)
{
	SFMLanimationManager &manager = _client->AnimManager();
	std::string data;
	data.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
	Animation anim = Animation::UnSerialize(data);

	std::string path(std::string(MAPDIR) + std::string(SLASHSEP) + Animation::FileName(anim));

	if (manager.IsLoad(path, anim.classid) == false)
	{
		manager.Load(path, path, anim.classid);
	}
	sf::Vector2<size_t> vec(anim.sizex, anim.sizey);

	manager.Load(path, anim.objname, anim.classid, anim.animid, vec, anim.index, anim.number, anim.duration);
	for (auto it = _example.begin(); it != _example.end(); it++)
	{
		if ((*it).ClassId() == anim.classid)
		{
			it = _example.erase(it);
			break;
		}
	}
	_example.push_back(GraphicalElem(anim.objname, anim.classid, 0, 0, 0, 0));
}

void ClientManager::handle_ask_map_succeed(TCPFrame*)
{
	if (CurrentMapDownload)
	{
		CurrentMapDownload->setDownloadState(MapDownloader::DOWNLOADING);
	}
}

void ClientManager::handle_ask_map_failed(TCPFrame*)
{
	if (CurrentMapDownload)
	{
		CurrentMapDownload->setDownloadState(MapDownloader::FAILED);
	}
}

void ClientManager::handle_map_download_finished(TCPFrame*)
{
	if (CurrentMapDownload)
	{
		CurrentMapDownload->setDownloadState(MapDownloader::FINISHED);
	}
}

void ClientManager::handle_game_loading(TCPFrame *)
{
	if (!CurrentGame && CurrentLobby && CurrentLobby->isConfirmed())
	{
		CurrentGame = new Game(CurrentLobby->getGameName(), CurrentLobby->getMapName());
		CurrentGame->setGameStatus(Game::LOADING);
	}
}

void ClientManager::handle_game_loading_finished(TCPFrame *)
{
	if (CurrentGame && CurrentLobby && CurrentLobby->isConfirmed())
		CurrentGame->setGameStatus(Game::LOADING_FINISH);
}

void ClientManager::handle_game_synchronization(TCPFrame *)
{

	if (CurrentGame && CurrentLobby && CurrentLobby->isConfirmed())
		CurrentGame->setGameStatus(Game::SYNCHRONIZATION);
}

void ClientManager::handle_game_synchronization_finished(TCPFrame *)
{
	if (CurrentGame && CurrentLobby && CurrentLobby->isConfirmed())
		CurrentGame->setGameStatus(Game::SYNCHRONIZATION_FINISHED);
}

void ClientManager::handle_game_started(TCPFrame *frame)
{
	if (CurrentGame && CurrentLobby && CurrentLobby->isConfirmed())
	{
		const char * msg = frame->getMessage();
		std::memcpy(&timepoint, msg, sizeof(std::chrono::high_resolution_clock::time_point));
		CurrentGame->setGameStatus(Game::GAME_STARTED);
	}
}

void ClientManager::handle_new_lobby_message(TCPFrame *frame)
{
	if (CurrentLobby == nullptr && CurrentLobby->isConfirmed() == false)
		return;
	std::string msg;
	msg.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
	CurrentLobby->Messages.push_back(msg);
}

void ClientManager::handle_ask_score(TCPFrame *frame)
{
	std::string msg;
	msg.insert(0, frame->getMessage(), frame->getHeader().getDataLength());

	Scores.clear();
	while (msg.size())
	{
		std::string name;
		std::string score;

		if ((name = popToken(msg, ";")) == "")
			return;
		if ((score = popToken(msg, ";")) == "")
			return;
		this->Scores.push_back(std::pair<std::string, std::string>(name, score));
	}
}


void ClientManager::handle_game_finished(TCPFrame *frame)
{
	if (this->CurrentGame)
	{
		CurrentGame->setGameStatus(Game::GAME_FINISHED);

		std::string score;

		score.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
		CurrentGame->setScore(std::atoi(score.c_str()));
	}
}

void ClientManager::askLobbyInformation()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LOBBY_INFORMATION);
}

void ClientManager::askAvailableMaps()
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	AvailableMaps.clear();
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::MAP_LIST);
}

void ClientManager::askAvailableGames()
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	AvailableGames.clear();
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::GAME_LIST);
}

void ClientManager::askToLaunch()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LAUNCH_GAME);
}

void ClientManager::askJoinGame(const std::string &gameName, const std::string &ownNickName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), gameName + ";" + ownNickName + ";", Protocole::Client::JOIN_GAME);
}

void ClientManager::askCreateGame(const std::string &gameName, const std::string &ownNickName, bool isPrivate)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	if (isPrivate == false)
		_msgFactory->SendMsg(_netClient.getOwnSession(), gameName + ";" + ownNickName + ";" + "public;", Protocole::Client::CREATE_GAME);
	else
		_msgFactory->SendMsg(_netClient.getOwnSession(), gameName + ";" + ownNickName + ";" + "private;", Protocole::Client::CREATE_GAME);
}

void ClientManager::askLeaveLobby()
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	try
	{
		_msgFactory->SendMsg(_netClient.getOwnSession(), "", Protocole::Client::LEAVE_LOBBY);
	}
	catch (std::exception &)
	{
	}
	if (CurrentLobby)
		delete CurrentLobby;
	CurrentLobby = nullptr;
}

void ClientManager::askMap(const std::string &mapName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), mapName + std::string(";"), Protocole::Client::ASK_MAP);
}

void ClientManager::askChangeMap(const std::string &mapName)
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), mapName + std::string(";"), Protocole::Client::GAME_CHANGE_MAP);
}

const std::list<GraphicalElem>	&ClientManager::getGraphicalElements() const
{
	return this->_example;
}

void ClientManager::askSendMap(const std::string &mapName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	std::string localPath = std::string(MAPDIR) + std::string(SLASHSEP) + mapName;
	_msgFactory->SendFile(_netClient.getOwnSession(), localPath, mapName, Protocole::Client::SEND_MAP);
}

void ClientManager::askSendLobbyMessage(const std::string &msg)
{
	if (_netClient.getOwnSessionID() == 0 || CurrentLobby == nullptr)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), msg, Protocole::Client::SEND_LOBBY_MESSAGE);
}

void ClientManager::askMapScore(const std::string &mapName)
{
	if (_netClient.getOwnSessionID() == 0)
		return;
	_msgFactory->SendMsg(_netClient.getOwnSession(), mapName + std::string(";"), Protocole::Client::ASK_SCORES);
}

const std::chrono::high_resolution_clock::time_point &ClientManager::getTimePoint() const
{
	return this->timepoint;
}

void	ClientManager::clearGraphicalElements()
{
	this->_example.clear();
}