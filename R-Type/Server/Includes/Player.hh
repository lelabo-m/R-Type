//
// Player.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Fri Nov 28 10:14:54 2014 Clément Roche
// Last update Sat Jan  3 11:13:19 2015 Clément Roche
//

#ifndef PLAYER_HH_
# define PLAYER_HH_

#include <string>
#include "IPlayer.hh"
#include "Client.hh"

class GameSession;

class Player : public IPlayer
{
public:
   enum player_state
   {
      WAITING,
      LOADING,
      READY,
      PLAYING
   };

private:
   Client				*_linkedClient;
   player_state			_state;
   const std::string	_name;
   size_t				_score;
   int					_id;
   bool					_master;

public:
   Player(void);
   explicit Player(Client *client, const std::string &);
   virtual ~Player(void);

   bool			isReady(void) const;
   bool			isMaster(void) const;
   void			setMaster(bool value);
   Client		*getClient(void) const;
   void			UpdateScores(size_t);
   size_t		GetScore(void) const;
   size_t		GetId() const;
   const std::string	&getPlayerName() const;
   void			updateState(const GameSession &session);
};

#endif
