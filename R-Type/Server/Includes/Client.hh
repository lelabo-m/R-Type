//
// Client.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Nov 26 20:19:28 2014 Clément Roche
// Last update Sat Dec 20 19:11:24 2014 Clément Roche
//

#ifndef CLIENT_HH_
# define CLIENT_HH_

#include <string>
#include <memory>
#include "Session.h"
#include "Values.h"

class GameSession;

class Client
{
private:
   unsigned int			_id;
   std::shared_ptr<Session>	_session;
   GameSession			*_currentGame;

public:
   explicit Client(std::shared_ptr<Session> &session);
   virtual ~Client(void);

   unsigned int	getID(void) const;
   bool		isConnected(void) const;
   bool		sendMessage(const std::string &content, Protocole::Server::Procedure);
   void		sendUDPMessage(UDPFrame *frame);
   unsigned int	receivedMessageCount(void) const;
   TCPFrame	*getReceivedMessage(void);
   unsigned int	receivedUDPCount(void) const;
   UDPFrame	*getReceivedUDP(void);
   void		setCurrentGame(GameSession *game);
   GameSession	*getCurrentGame(void) const;
   std::shared_ptr<Session> getClientSession();
};

#endif
