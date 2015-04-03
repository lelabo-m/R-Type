//
// GameSession.cpp for Sources in /home/pailli_a/Projects/TFS/Projets_Epitech/R-Type/Server/Sources
//
// Made by Clément Roche
// Login   <roched@epitech.net>
//
// Started on  Fri Nov 28 10:25:58 2014 Clément Roche
// Last update Mon Jan  5 20:26:15 2015 Clément Roche
//

#include <chrono>
#include <iostream>
#include <ctime>
#include "GameSession.hh"
#include "ResourcesPackageServer.h"
#include "Server.hh"
#include "GameMessage.h"
#include "GameMessageDispacher.hpp"
#include "ObjectRepresentation.h"

GameSession::GameSession(PluginStorage &p, Server &server, GameSession::gamesession_type type) : _state(NOMASTER), _maxPlayers(NB_PLAYERS_MAX), _thread(0), _serverStop(false), _mapName("UNDEFINED"), _pStorage(p), _context(0), _server(server), _sessionType(type)
{
}

GameSession::~GameSession(void)
{
   std::cout << "DESTRUCTOR of GAMESESSION" << std::endl;
   _serverStop = true;
   _state = ENDED;
   if (_thread)
   {
      _thread->join();
      delete _thread;
   }
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      Player *player = *it;
      it = _players.erase(it);
      delete player;
   }
   if (_context)
      delete _context;
}

bool	GameSession::addClient(Client *client, const std::string &name)
{
   if (!client || _players.size() == (unsigned int)_maxPlayers
       || getState() != GameSession::WAITING)
      return (false);
   Player *p = new Player(client, name);
   client->setCurrentGame(this);
   _players.push_back(p);
   return true;
}

bool	GameSession::addMasterClient(Client *client, const std::string &name)
{
   if (!client || _players.size() != 0)
      return false;
   _state = WAITING;
   addClient(client, name);
   auto it = _players.begin();
   if (it == _players.end())
      return false;
   (*it)->setMaster(true);
   return true;
}

bool GameSession::loading()
{
   std::cout << "LOADING of GameSession map level file name : " << _mapName << std::endl;
   // faudra changer la size de la map
   _context = new GameContext(875, 1280, _pStorage.buildFactory(), new GameMessagePool(42));
   FileToSend toSend(MAPDIR + std::string(SLASHSEP) + _mapName, _mapName);
   ResourcesPackage *rp = _pStorage.buildPackage(toSend, PackageType::GAME);
   _playersLock.lock();
   for (auto it : _players)
   {
      try
      {
	 std::shared_ptr<Session> clientSession = it->getClient()->getClientSession();
	 rp->sendToPlayer(clientSession);
      }
      catch (std::exception &)
      {
	 std::cerr << "An error occured while trying to send resources to client" << std::endl;
      }
   }
   _playersLock.unlock();
   // All players downloaded the map
   return waitLoadingPlayers(rp);
}

bool	GameSession::waitLoadingPlayers(ResourcesPackage *rp)
{
   bool finished = false;

   while (finished == false)
   {
      finished = true;
      _playersLock.lock();
      std::chrono::milliseconds d(500);
      for (auto it = _players.begin(); it != _players.end(); it++)
      {
	 std::shared_ptr<Session> session = (*it)->getClient()->getClientSession();
	 if (!rp->hasPlayerLoaded(session))
	    finished = false;
	 else
	 {
	    (*it)->getClient()->sendMessage("", Protocole::Server::GAME_LOADING_FINISHED);
	    (*it)->getClient()->sendMessage("", Protocole::Server::GAME_SYNCHRONIZATION);
	 }
	 std::this_thread::sleep_for(d);
      }
      _playersLock.unlock();
      if (_serverStop == true)
      {
	 delete rp;
	 return false;
      }
   }
   delete rp;
   return true;
}

void	GameSession::run(void)
{
   _state = LOADING;
   allClientsUpdateState();
   sendAll("", Protocole::Server::GAME_LOADING);
   if (!loading())
      return;
   _state = PLAYING;
   allClientsUpdateState();
   sendAll("", Protocole::Server::GAME_SYNCHRONIZATION_FINISHED);

   if (!_serverStop && _state != ENDED && _context)
   {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      sendAll("", Protocole::Server::GAME_STARTED);
      std::chrono::high_resolution_clock::time_point tstart = std::chrono::high_resolution_clock::now();
      std::cout << "A game is running !" << std::endl;
      play(tstart);
   }
   _state = ENDED;
}

void	GameSession::play(std::chrono::high_resolution_clock::time_point &tstart)
{
	_playersLock.lock();
	_context->Start(_mapName, GameContext::RtypeGame(), _players);
	_playersLock.unlock();

	std::list<GameMessage *> list;

	std::cout << "WAITING" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "REALLY STARTED" << std::endl;
	while (_serverStop == false && _state == PLAYING)
	{
		std::chrono::high_resolution_clock::time_point beginCycle = std::chrono::high_resolution_clock::now();

		_playersLock.lock();
		for (auto it : _players)
		{
			for (unsigned int i = it->getClient()->receivedUDPCount(); i > 0; i--)
			{
				UDPFrame *f = it->getClient()->getReceivedUDP();
				f->getHeader().setFromData(f->getData());
				//std::cout << "Server cycle: " << std::chrono::duration_cast<std::chrono::milliseconds>(beginCycle - tstart).count() / TICK_DURATION << std::endl;
				//std::cout << "Client cycle: " << f->getHeader().getTime() << std::endl;

				GameMessageDispacher::extractFrame(f, list, it->GetId(), f->getHeader().getTime());
				f->release();
			}
		}
		_context->In().insert(_context->In().end(), list.begin(), list.end());
		_context->Update();
		_context->In().clear();
		_playersLock.unlock();
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

		while (_context->Out().size())
		{
			UDPFrame *sendFrame = GameMessageDispacher::fillFrame(_pool.createUDPFrame(), _context->Out(), _context->MessagePool());
			sendFrame->getHeader().setMagicNumber(42);
			int time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - tstart).count();
			int cycle = time / TICK_DURATION;

			sendFrame->getHeader().setTime(cycle);
			sendAllUDP(sendFrame);
		}
		list.clear();
		if (_context->IsFinish())
			this->endGame();
		std::chrono::high_resolution_clock::time_point tend = std::chrono::high_resolution_clock::now();
		auto delay = std::chrono::duration_cast<std::chrono::microseconds>(tend - beginCycle);

		if (delay < UPDATE_DURATION)
			std::this_thread::sleep_for(UPDATE_DURATION - delay);
	}
}

bool	GameSession::startGame(void)
{
   if (_state != WAITING)
      return false;
   _thread = new std::thread(&GameSession::run, this);
   if (!_thread)
      return false;
   return true;
}

void	GameSession::saveScores(void)
{
   for (auto it : _players)
   {
      _server.saveScore(_mapName, it->getPlayerName(), (int)it->GetScore());
   }
}

void	GameSession::endGame(void)
{
   if (_state != ENDED)
      _state = ENDED;
   saveScores();
   for (auto it = _players.begin(); it != _players.end();)
   {
	   (*it)->getClient()->sendMessage(std::to_string((*it)->GetScore()) + ";", Protocole::Server::GAME_FINISHED);
      delete (*it);
      it = _players.erase(it);
   }
}

bool	GameSession::allClientsReady(void) const
{
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      if (!(*it)->isReady())
	 return false;
   }
   return true;
}

void	GameSession::allClientsUpdateState(void)
{
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      (*it)->updateState(*this);
   }
}

GameSession::gamesession_state	GameSession::getState(void) const
{
   return _state;
}

void	GameSession::stop(void)
{
   _serverStop = true;
}

std::string	GameSession::getName(void) const
{
   return _gameName;
}

void		GameSession::setName(std::string name)
{
   _gameName = name;
}

void	GameSession::notify(Client *client, gamesession_event type)
{
   std::cout << "notify" << std::endl;
   if (!client)
      return;
   if (type == DISCONNECT)
   {
      if (isMaster(client))
      {
	 removeClient(client);
	 _state = ENDED;
	 sendAll("This game has been deleted !", Protocole::Server::KICK_FROM_LOBBY);
	 std::cout << "notify end" << std::endl;
	 endGame();
      }
      else
      {
	 removeClient(client);
	 sendAllLobbyInfo();
      }
      std::cout << "notify ended" << std::endl;
   }
}

void	GameSession::sendAll(std::string content, Protocole::Server::Procedure procedure)
{
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      if (*it && (*it)->getClient())
	 (*it)->getClient()->sendMessage(content, procedure);
   }
}

Player	*GameSession::getMaster(void) const
{
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      if (*it && (*it)->isMaster())
	 return (*it);
   }
   return (0);
}

unsigned int GameSession::playerCount() const
{
   return _players.size();
}

const std::string &GameSession::getMapName() const
{
   return _mapName;
}

void GameSession::setMapName(const std::string &str)
{
   _mapName = str;
}

std::list<std::string> GameSession::getPlayersNames() const
{
   std::list<std::string> list;

   for (auto it : _players)
   {
      list.push_back(it->getPlayerName());
   }
   return list;
}

void	GameSession::sendAllLobbyInfo(void)
{
   std::string rep;
   rep = getName() + "," + getMapName() + ",";
   rep += getPlayersNamesAsString();
   sendAll(rep, Protocole::Server::LOBBY_INFORMATION);
}

void	GameSession::sendAllUDP(UDPFrame *frame)
{
	_playersLock.lock();
	for (auto it = _players.begin(); it != _players.end(); it++)
	{
		UDPFrame *new_frame = _pool.createUDPFrame();
		*new_frame = *frame;
		new_frame->getHeader().setSessionID((*it)->getClient()->getClientSession().get()->getSessionID());
		new_frame->getHeader().setDataFromHeader(new_frame->getData());
		(*it)->getClient()->sendUDPMessage(new_frame);
	}
	frame->release();
	_playersLock.unlock();
}

bool	GameSession::isMaster(Client *client) const
{
   if (Player *p = getMaster())
      return (client == p->getClient());
   return false;
}

std::string	GameSession::getPlayersNamesAsString(char separator)
{
   std::string ret;

   for (auto it : _players)
   {
      ret += it->getPlayerName() + separator;
   }
   return (ret);
}

void	GameSession::removeClient(Client *client)
{
   if (!client)
      return;
   _playersLock.lock();
   for (auto it = _players.begin(); it != _players.end(); ++it)
   {
      if ((*it)->getClient() == client)
      {
	 if (_context && _state == PLAYING)
	    _context->PlayerDie(*it);
	 client->setCurrentGame(0);
	 delete *it;
	 _players.erase(it);
	 _playersLock.unlock();
	 return;
      }
   }
   _playersLock.unlock();
}

bool	GameSession::isDeletable(void) const
{
   if (_players.size() == 0 && _state == ENDED)
      return true;
   return false;
}

const std::string	GameSession::getPlayerNameFromClient(Client *client)
{
   if (client)
   {
      for (auto it : _players)
      {
	 if (it->getClient() == client)
	 {
	    return it->getPlayerName();
	 }
      }
   }
   return ("");
}

GameSession::gamesession_type	GameSession::getSessionType() const
{
	return _sessionType;
}
