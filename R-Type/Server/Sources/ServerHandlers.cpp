//
// ServerHandlers.cpp for Sources in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Server/Sources
//
// Made by Clément Roche
// Login   <roched@epitech.net>
//
// Started on  Sun Dec 14 20:47:13 2014 Clément Roche
// Last update Mon Jan  5 20:54:50 2015 Clément Roche
//

#include <iostream>
#include "Server.hh"
#include "Utilities.hpp"

bool	Server::handleInitConnection(Client *, TCPFrame *)
{
   return true;
}

bool	Server::handleGameList(Client *client, TCPFrame *)
{
   if (!client)
      return false;
   std::string rep;
   unsigned int count = 0;
   for (auto it = _games.begin(); it != _games.end(); ++it)
   {
	   if ((*it).second->getState() == GameSession::WAITING && (*it).second->getSessionType() == GameSession::gamesession_type::PUBLIC)
      {
	 rep += ((*it).second->getName() + ",") + (*it).second->getMapName() + "," + std::to_string((*it).second->playerCount()) + ";";
	 ++count;
      }
   }
   client->sendMessage(rep, Protocole::Server::GAME_LIST);
   return true;
}

bool	Server::handleJoinGame(Client *client, TCPFrame *frame)
{
   std::string token;
   if (!client || !frame)
      return false;
   std::cout << "Join Game " << frame->getMessage() << std::endl;
   token = frame->getMessage();
   
   std::string name(popToken(token, ";"));
   std::string playerName(popToken(token, ";"));

   if (name == "" || playerName == "")
   {
      client->sendMessage("No game name or nickname", Protocole::Server::JOIN_GAME_FAILED);
      return true;
   }
   
   GameSession *session = getGameSessionByName(name);
   if (!session)
   {
      client->sendMessage("This game does not exist !", Protocole::Server::JOIN_GAME_FAILED);
      return true;
   }
   if (session->getState() == GameSession::WAITING)
   {
      if (session->addClient(client, playerName))
      {
	 client->sendMessage("", Protocole::Server::JOIN_GAME_SUCCESS);
	 std::string mapName = session->getMapName();
	 std::string toSend = session->getName() + "," + mapName + ",";
	 toSend += session->getPlayersNamesAsString();
	 session->sendAll(toSend, Protocole::Server::LOBBY_INFORMATION);
      }
      else
	 client->sendMessage("Impossible to join the game !", Protocole::Server::JOIN_GAME_FAILED);
   }
   else
      client->sendMessage("This game has already started !", Protocole::Server::JOIN_GAME_FAILED);
   return true;
}

bool	Server::handleCreateGame(Client *client, TCPFrame *frame)
{
   std::string token;
   if (!client || !frame)
      return false;
   std::cout << "[HANDLE] [" << client->getID() << "] Create Game " << frame->getMessage() << std::endl;
   token = frame->getMessage();


   std::string name(popToken(token, ";"));
   std::string ownerName(popToken(token, ";"));
   std::string gameType(popToken(token, ";"));
   if (name == "" || ownerName == "")
   {
      client->sendMessage("No game name or nickname", Protocole::Server::GAME_CREATION_FAILED);
      return true;
   }
   
   GameSession *session = getGameSessionByName(name);

   GameSession::gamesession_type type = GameSession::PUBLIC;
   if (gameType == std::string("private"))
   {
	   name = name + ";" + ownerName;
	   type = GameSession::PRIVATE;
   }

   if (session)
   {
      client->sendMessage("This game already exist !", Protocole::Server::GAME_CREATION_FAILED);
      return true;
   }
   if (!createGame(client, name, ownerName, type))
      client->sendMessage("Server was unable to create the game !", Protocole::Server::GAME_CREATION_FAILED);
   else
      client->sendMessage("", Protocole::Server::GAME_CREATION_SUCCESS);
   return true;
}

bool	Server::handleDeleteGame(Client *client, TCPFrame *)
{
   GameSession *session = client->getCurrentGame();
   if (!session)
      client->sendMessage("This player didn't join any game !", Protocole::Server::FAILURE);
   else
   {
      if (session->isMaster(client))
	 session->notify(client, GameSession::DISCONNECT);
      else
	 client->sendMessage("This player is not the master of the game !", Protocole::Server::FAILURE);
   }
   return true;
}

bool	Server::handleSendLobbyMessage(Client *client, TCPFrame *frame)
{
   GameSession *session = client->getCurrentGame();
   if (session)
   {
      std::string rep(session->getPlayerNameFromClient(client));
      rep += " :";
      rep.insert(rep.size(), frame->getMessage(), frame->getHeader().getDataLength());
      session->sendAll(rep, Protocole::Server::NEW_LOBBY_MESSAGE);
   }
   return true;
}

bool	Server::handleSendFriendMessage(Client *, TCPFrame *)
{
   return true;
}

bool	Server::handleDisconnect(Client *, TCPFrame *)
{
   return true;
}

bool	Server::handleLaunchGame(Client *client, TCPFrame *)
{
   std::cout << "[HANDLER] ASKED TO LAUNCH GAME !" << std::endl;
   GameSession *session = client->getCurrentGame();
   if (!session)
      client->sendMessage("This player didn't join any game !", Protocole::Server::FAILURE);
   else
   {
      if (session->isMaster(client))
      {
	 if (!session->startGame())
	    client->sendMessage("Impossible to start the game !", Protocole::Server::FAILURE);
      }
      else
	 client->sendMessage("This player is not the master of the game !", Protocole::Server::FAILURE);
   }
   return true;
}

bool	Server::handleMapList(Client *client, TCPFrame *frame)
{
   if (frame->getHeader().getProcedureCode() == Protocole::Client::MAP_LIST)
   {
      std::string message = Utilities::getFilesNames_directory(MAPDIR, "*");
      client->sendMessage(message, Protocole::Server::MAP_LIST);
   }
   return true;
}

bool	Server::handleLeaveLobby(Client *client, TCPFrame *)
{
   GameSession *session = client->getCurrentGame();
   if (!session)
      client->sendMessage("This player didn't join any game !", Protocole::Server::FAILURE);
   else
   {
      session->notify(client, GameSession::DISCONNECT);
      std::string rep;
      rep = session->getName() + "," + session->getMapName() + ",";
      rep += session->getPlayersNamesAsString();
      client->sendMessage(rep, Protocole::Server::LOBBY_INFORMATION);
   }
   return true;
}

bool	Server::handleLobbyInformation(Client *client, TCPFrame *)
{
   GameSession *session = client->getCurrentGame();
   if (session)
   {
      std::string rep;

	  std::string gameName = session->getName();
	  if (session->getSessionType() == GameSession::PRIVATE)
		  gameName = popToken(gameName, ";");

	  rep = gameName + "," + session->getMapName() + ",";
      rep += session->getPlayersNamesAsString();
      client->sendMessage(rep, Protocole::Server::LOBBY_INFORMATION);
   }
   return true;
}

bool	Server::handleAskMap(Client *client, TCPFrame *frame)
{
   std::string mapname;
   std::shared_ptr<Session> clientSession;
   mapname.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
   mapname = popToken(mapname, ";");
   
   if (mapname.size() != 0)
   {
      if (FileSystem::TypeOfFile(MAPDIR + std::string(SLASHSEP) + mapname) != FileSystem::FileType::REGULAR)
      {
	 client->sendMessage("Map doesn't exist", Protocole::Server::ASK_MAP_FAILED);
	 return true;
      }
   }
   client->sendMessage("", Protocole::Server::ASK_MAP_SUCCEED);
   FileToSend toSend(MAPDIR + std::string(SLASHSEP) + mapname, mapname);
   ResourcesPackage *rp = _pluginStorage.buildPackage(toSend, PackageType::EDITOR);
   clientSession = client->getClientSession();
   rp->sendToPlayer(clientSession);
   client->sendMessage("", Protocole::Server::MAP_DOWNLOAD_FINISH);
   return true;
}

bool	Server::handleChangeMap(Client *client, TCPFrame *frame)
{
   GameSession *session = client->getCurrentGame();

   if (session && session->isMaster(client))
   {
      std::string mapName;
      mapName.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
      mapName = popToken(mapName, ";");
      session->setMapName(mapName);

	  std::string gameName = session->getName();
	  if (session->getSessionType() == GameSession::PRIVATE)
		  gameName = popToken(gameName, ";");

	  std::string toSend = gameName + "," + mapName + ",";
      toSend += session->getPlayersNamesAsString(',');
      session->sendAll(toSend, Protocole::Server::LOBBY_INFORMATION);
   }
   return true;
}

bool	Server::handleSendMap(Client *, TCPFrame *frame)
{
   std::string mapName;
   std::string data;

   mapName = frame->getMessage();
   data.insert(0, &frame->getMessage()[mapName.size() + 1], frame->getHeader().getDataLength() - mapName.size() - 1);
   std::string path = std::string(MAPDIR) + std::string(SLASHSEP) + mapName;
   FileManager::writeFile(path, data);
   return true;
}

bool	Server::handleAskScoresMap(Client *client, TCPFrame *frame)
{
   if (!client || !frame)
      return false;
   std::string mapName;
   mapName.insert(0, frame->getMessage(), frame->getHeader().getDataLength());
   mapName = popToken(mapName, ";");
   std::string rep;
   try
   {
      auto map = _scores.getScore(mapName);
      for (auto it : map)
      {
	 rep += it.first;
	 rep += ";";
	 rep += std::to_string(it.second);
	 rep += ";";
      }
      client->sendMessage(rep, Protocole::Server::ASK_MAP_SCORES);
   }
   catch (std::exception &)
   {
      client->sendMessage("No saved scores for this map !", Protocole::Server::FAILURE);
   }
   return true;
}

void	Server::initHandlers(void)
{
   _tcpHandlers.push_back(TCPHandler(0, "init", BIND(&Server::handleInitConnection)));
   _tcpHandlers.push_back(TCPHandler(1, "gamelist", BIND(&Server::handleGameList)));
   _tcpHandlers.push_back(TCPHandler(2, "joingame", BIND(&Server::handleJoinGame)));
   _tcpHandlers.push_back(TCPHandler(3, "creategame", BIND(&Server::handleCreateGame)));
   _tcpHandlers.push_back(TCPHandler(4, "deletegame", BIND(&Server::handleDeleteGame)));
   _tcpHandlers.push_back(TCPHandler(5, "sendlobbymessage", BIND(&Server::handleSendLobbyMessage)));
   _tcpHandlers.push_back(TCPHandler(6, "sendfriendmessage", BIND(&Server::handleSendFriendMessage)));
   _tcpHandlers.push_back(TCPHandler(7, "disconnect", BIND(&Server::handleDisconnect)));
   _tcpHandlers.push_back(TCPHandler(8, "maplist", BIND(&Server::handleMapList)));
   _tcpHandlers.push_back(TCPHandler(9, "launchgame", BIND(&Server::handleLaunchGame)));
   _tcpHandlers.push_back(TCPHandler(10, "lobbyinformation", BIND(&Server::handleLobbyInformation)));
   _tcpHandlers.push_back(TCPHandler(11, "leavelobby", BIND(&Server::handleLeaveLobby)));
   _tcpHandlers.push_back(TCPHandler(13, "askmap", BIND(&Server::handleAskMap)));
   _tcpHandlers.push_back(TCPHandler(14, "changemap", BIND(&Server::handleChangeMap)));
   _tcpHandlers.push_back(TCPHandler(15, "sendmap", BIND(&Server::handleSendMap)));
   _tcpHandlers.push_back(TCPHandler(16, "askscores", BIND(&Server::handleAskScoresMap)));
}
