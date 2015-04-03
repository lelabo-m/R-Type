//
// GameSession.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Fri Nov 28 10:23:02 2014 Clément Roche
// Last update Sat Jan  3 11:11:46 2015 Clément Roche
//

#ifndef GAMESESSION_HH_
# define GAMESESSION_HH_

#include <list>
#include <thread>
#include <mutex>
#include <string>
#include "Client.hh"
#include "Player.hh"
#include "Values.h"
#include "PluginStorage.hh"
#include "GameContainer.h"

#define	NB_PLAYERS_MAX	4
#define TICK_PER_SEC 50
#define UPDATE_DURATION std::chrono::microseconds((1000000 / TICK_PER_SEC))
#define TICK_DURATION (1000 / TICK_PER_SEC)

class Scores;
class Server;

class GameSession
{
public:
   enum gamesession_state
   {
      NOMASTER,
      WAITING,
      LOADING,
      PLAYING,
      ENDED
   };

   enum gamesession_event
   {
      DISCONNECT
   };

   enum gamesession_type
   {
	   PUBLIC,
	   PRIVATE
   };

private:
   gamesession_state	_state;
   int			_maxPlayers;
   std::list<Player *>	_players;
   std::mutex		_playersLock;
   std::thread		*_thread;
   bool			_serverStop;
   std::string		_gameName;
   std::string		_mapName;
   PluginStorage	&_pStorage;
   GameContext		*_context;
   UDPFramePool		_pool;
   Server		&_server;
   gamesession_type _sessionType;

public:
   explicit GameSession(PluginStorage &, Server &, gamesession_type = PUBLIC);
   virtual ~GameSession(void);

   bool				addClient(Client *client, const std::string &);
   bool				addMasterClient(Client *client, const std::string &);
   bool				startGame(void);
   void				endGame(void);
   bool				allClientsReady(void) const;
   void				allClientsUpdateState(void);
   gamesession_state		getState(void) const;
   void				run(void);
   bool				loading(void);
   bool				waitLoadingPlayers(ResourcesPackage*);
   void				stop(void);
   void				play(std::chrono::high_resolution_clock::time_point &);
   std::string			getName(void) const;
   void				setMapName(const std::string &str);
   void				setName(std::string name);
   void				notify(Client *client, gamesession_event type);
   void				sendAll(std::string content, Protocole::Server::Procedure);
   void				sendAllLobbyInfo(void);
   void				sendAllUDP(UDPFrame *frame);
   Player			*getMaster(void) const;
   unsigned int			playerCount(void) const;
   const std::string		&getMapName(void) const;
   std::list<std::string>	getPlayersNames(void) const;
   bool				isMaster(Client *client) const;
   std::string			getPlayersNamesAsString(char separator = ',');
   const std::string		getPlayerNameFromClient(Client *client);
   void				removeClient(Client *client);
   bool				isDeletable(void) const;
   void				saveScores(void);
   gamesession_type	getSessionType() const;

};

#endif
