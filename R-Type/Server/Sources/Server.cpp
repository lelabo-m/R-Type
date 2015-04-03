//
// Server.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Nov 26 19:53:44 2014 Clément Roche
// Last update Mon Jan  5 21:03:20 2015 Clément Roche
//

#include <iostream>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "Interrupt.hh"
#include "Server.hh"
#include "FileSystem.hpp"
#include <list>
#include <string>

Server::Server(void) : _state(STOPPED), _port(6666)
{
   initHandlers();
   _scoreFile = "highscores.xml";
   std::cout << "Server ready !" << std::endl;
}

Server::~Server(void)
{
}

std::string Server::popToken(std::string &str, const std::string &delim) const
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

bool	Server::start(void)
{
   if (FileSystem::TypeOfFile(PLUGINDIR) != FileSystem::FileType::DIRECTORY)
   {
      std::cerr << "Not plugin directory found! (" << PLUGINDIR << ")" << std::endl;
      return false;
   }
   if (FileSystem::TypeOfFile(MAPDIR) != FileSystem::FileType::DIRECTORY)
   {
      std::cerr << "Not map directory found! (" << MAPDIR << ")" << std::endl;
      return false;
   }
   _pluginStorage.initialize(PLUGINDIR);
   setInterrupt(this);
   _scores.load(_scoreFile);
   _netManager.makeOnline((unsigned short)_port);
   _state = STARTED;
   std::cout << "Server Started !" << std::endl;
   loop();
   std::cout << "Server Closing !" << std::endl;
   _netManager.makeOffline();
   _scores.save(_scoreFile);
   return true;
}

void	Server::stop(void)
{
   if (_state == STOPPED)
      return;
   _state = STOPPED;
   stopRemainingGames();
}

int	Server::loopSession(void)
{
   int r = 0;
   for (auto it = _games.begin(); it != _games.end();)
   {
      if ((*it).second && (*it).second->isDeletable())
      {
	 GameSession *game = (*it).second;
	 it = _games.erase(it);
	 delete game;
	 ++r;
      }
      else
      {
	 // DispachUDPFrame
	 ++it;
      }
   }
   return (r);
}

int	Server::loopNewClients(unsigned int toConnect)
{
   unsigned int r = 0;
   bool disco = true;

   std::list<std::shared_ptr<Session> > netSessions = _netManager.getFreshSessions();
   while (r < toConnect && disco)
   {
      if (netSessions.begin() != netSessions.end())
      {
	 registerClient(netSessions.front());
	 netSessions.pop_front();
	 ++r;
      }
      else
	 disco = false;
   }
   return (int)r;
}

void	Server::loopCommands(void)
{
   while (_cmdLine.getCommandCount())
   {
      std::string cmd = _cmdLine.getLastCommand();
      if (cmd == "exit")
	 stop();
      else if (cmd == "savescore")
	 _scores.save(_scoreFile);
      else if (cmd == "gamelist")
      {
	 std::cout << "GAME LIST:" << std::endl;
	 for (auto it = _games.begin(); it != _games.end(); ++it)
	 {
	    std::cout << "GAME : " << (*it).second->getName()
		      << "\t\tstate: " << (*it).second->getState()
		      << "\tplayers:" << (*it).second->playerCount()
		      << "\tlevel:" << (*it).second->getMapName() << std::endl;
	 }
      }
      else
	 std::cout << "Unknown command : " << cmd << std::endl;
   }
}

void	Server::updateWaitingClient(Client *client)
{
   if (!client->isConnected())
      return;
   for (unsigned int count = client->receivedMessageCount(); count > 0; --count)
   {
      if (TCPFrame *message = client->getReceivedMessage())
      {
	 try
	 {
	    handleTCPRequest(client, message);
	 }
	 catch (std::exception &e)
	 {
	    std::cerr << "Unable to handle a request : " << e.what() << std::endl;
	 }
	 delete message;
      }
   }
}

bool	Server::handleTCPRequest(Client *client, TCPFrame *frame)
{
   auto it = std::find_if(_tcpHandlers.begin(), _tcpHandlers.end(), [frame, this, client](TCPHandler &ref)
			  {
			     if (ref.getProcedureCode() == frame->getHeader().getProcedureCode())
			     {
				ref(*this, client, frame);
				return (true);
			     }
			     return (false);
			  });
   if (it == _tcpHandlers.end())
      throw std::runtime_error("No handler exists for this request !");
   return true;
}

bool	Server::handleTCPRequest(Client *client, std::string handleName)
{
   auto it = std::find_if(_tcpHandlers.begin(), _tcpHandlers.end(), [handleName, this, client](TCPHandler &ref)
			  {
			     if (ref.getName() == handleName)
				return (ref(*this, client, 0));
			     return (false);
			  });
   if (it == _tcpHandlers.end())
      throw std::runtime_error("No handler exists for this request !");
   return true;
}

int	Server::loopWaiting(void)
{
   bool rm = false;
   for (auto it = _clientsWaiting.begin(); it != _clientsWaiting.end();)
   {
      rm = false;
      if (*it)
      {
	 if ((*it)->isConnected())
	    updateWaitingClient(*it);
	 else
	 {
	    std::cout << "loopWaiting with a disconnected client !" << std::endl;
	    GameSession *session = (*it)->getCurrentGame();
	    if (session == 0)
	    {
	       delete *it;
	       rm = true;
	    }
	    else
	       session->notify(*it, GameSession::DISCONNECT);
	 }
      }
      else
	 rm = true;
      (rm) ? (it = _clientsWaiting.erase(it)) : (++it);
   }
   return (0);
}

void Server::dispachUDP(void)
{
}

int	Server::loop(void)
{
	int i = 0;
	std::chrono::milliseconds d(1000);
	while (_state == STARTED)
	{
		loopCommands();
		std::this_thread::sleep_for(d);
		loopWaiting();
		dispachUDP();
		loopSession();
		if (_netManager.countFreshSessions())
			loopNewClients(_netManager.countFreshSessions());
		++i;
	}
	return (0);
}

bool	Server::isRunning(void) const
{
   if (_state == STARTED)
      return true;
   return false;
}

void	Server::setPort(int port)
{
   if (port <= 0)
      throw std::runtime_error("Invalid port number !");
   _port = port;
}

void	Server::stopRemainingGames(void)
{
   if (_games.size() <= 0)
      return;
   for (auto it = _games.begin(); it != _games.end(); ++it)
   {
      if ((*it).second)
	 (*it).second->stop();
   }
}

bool	Server::registerClient(std::shared_ptr<Session> session)
{
   _clientsWaiting.insert(_clientsWaiting.begin(), new Client(session));
   return true;
}

unsigned int	Server::getNumberGameWithState(GameSession::gamesession_state state)
{
   unsigned int ret = 0;
   for (auto it = _games.begin(); it != _games.end(); ++it)
   {
      if ((*it).second->getState() == state)
	 ++ret;
   }
   return ret;
}

bool	Server::createGame(Client *client, std::string gameName, const std::string &ownerName, GameSession::gamesession_type t)
{
   if (!client)
      return false;
   GameSession *session = new GameSession(_pluginStorage, *this, t);
   session->setName(gameName);
   session->addMasterClient(client, ownerName);

   unsigned char			nextGameID = 0;
   while (_games.count(nextGameID) > 0 && nextGameID < 255)
      nextGameID++;
   if (nextGameID == 255)
   {
      delete session;
      return false;
   }
   _games[nextGameID] = session;

   return true;
}

GameSession	*Server::getGameSessionByName(std::string name)
{
   for (auto it = _games.begin(); it != _games.end(); ++it)
   {
      if ((*it).second->getName() == name)
	 return ((*it).second);
   }
   return (0);
}

void	Server::saveScore(const std::string &mapname, const std::string &playername, int score)
{
   std::lock_guard<std::mutex> lock(_scoresLock);
   _scores.setScore(mapname, playername, score);
}
