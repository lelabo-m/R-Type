//
// Server.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Nov 26 19:52:22 2014 Clément Roche
// Last update Sat Jan  3 14:11:14 2015 Clément Roche
//

#ifndef SERVER_HH_
# define SERVER_HH_

#include <vector>
#include <list>
#include <mutex>
#include "Client.hh"
#include "GameSession.hh"
#include "Session.h"
#include "NetServerManager.h"
#include "CommandLine.hh"
#include "TCPHandler.hh"
#include "PluginStorage.hh"
#include "GameMessage.h"
#include "Scores.hh"

#define PLUGINDIR	"plugin"
#define MAPDIR		"map"

class Server
{
public:
   enum state
   {
      STARTED,
      STOPPED
   };

private:
   enum state					_state;
   std::vector<Client *>			_clientsWaiting;
   std::vector<Client *>			_clientsPlaying;
   std::map<unsigned char, GameSession *>	_games;
   int						_port;
   NetServerManager				_netManager;
   CommandLine					_cmdLine;
   std::list<TCPHandler>			_tcpHandlers;
   PluginStorage				_pluginStorage;
   Scores					_scores;
   std::mutex					_scoresLock;
   std::string					_scoreFile;

   bool				handleInitConnection(Client *client, TCPFrame *frame);
   bool				handleGameList(Client *client, TCPFrame *frame);
   bool				handleMapList(Client *client, TCPFrame *frame);
   bool				handleJoinGame(Client *client, TCPFrame *frame);
   bool				handleCreateGame(Client *client, TCPFrame *frame);
   bool				handleDeleteGame(Client *client, TCPFrame *frame);
   bool				handleSendLobbyMessage(Client *client, TCPFrame *frame);
   bool				handleSendFriendMessage(Client *client, TCPFrame *frame);
   bool				handleDisconnect(Client *client, TCPFrame *frame);
   bool				handleLaunchGame(Client *client, TCPFrame *frame);
   bool				handleLeaveLobby(Client *client, TCPFrame *frame);
   bool				handleLobbyInformation(Client *client, TCPFrame *frame);
   bool				handleAskMap(Client *client, TCPFrame *frame);
   bool				handleChangeMap(Client *client, TCPFrame *frame);
   bool				handleSendMap(Client *client, TCPFrame *frame);
   bool				handleAskScoresMap(Client *client, TCPFrame *frame);

protected:
   std::string		popToken(std::string &str, const std::string &delim) const;
   void			dispachUDP(void);
   int			loop(void);
   int			loopSession(void);
   int			loopWaiting(void);
   int			loopNewClients(unsigned int toConnect);
   void			loopCommands(void);
   void			stopRemainingGames(void);
   bool			registerClient(std::shared_ptr<Session> session);
   void			updateWaitingClient(Client *client);
   bool			handleTCPRequest(Client *client, TCPFrame *frame);
   bool			handleTCPRequest(Client *client, std::string handleName);
   void			initHandlers(void);
   bool			createGame(Client *client, std::string gameName, const std::string &, GameSession::gamesession_type t = GameSession::PUBLIC);

public:
   explicit Server(void);
   virtual ~Server(void);

   bool			start(void);
   void			stop(void);

   bool			isRunning(void) const;
   void			setPort(int port);
   unsigned int	        getNumberGameWithState(GameSession::gamesession_state state);
   GameSession		*getGameSessionByName(std::string name);
   void			saveScore(const std::string &mapname, const std::string &playername, int score);
};

#endif
