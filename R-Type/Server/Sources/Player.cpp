//
// Player.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Fri Nov 28 10:15:51 2014 Clément Roche
// Last update Mon Jan  5 14:45:17 2015 Clément Roche
//

#include <iostream>
#include "GameSession.hh"
#include "Player.hh"

Player::Player(void) : Player(0, "")
{
}

Player::Player(Client *client, const std::string &name) : _linkedClient(client), _state(Player::WAITING), _name(name), _score(0), _master(false)
{
   _id = _linkedClient->getID();
}

Player::~Player(void)
{
   std::cout << "PLAYER DESTRUCTOR ! " << std::endl;
   if (_linkedClient)
      _linkedClient->setCurrentGame(0);
}

bool	Player::isReady(void) const
{
   return (_state == Player::READY);
}

bool	Player::isMaster(void) const
{
   return _master;
}

void	Player::setMaster(bool value)
{
   _master = value;
}

Client	*Player::getClient(void) const
{
   return _linkedClient;
}

void	Player::UpdateScores(size_t val)
{
	_score += val;
}

size_t	Player::GetId() const
{
	return _id;
}

const std::string	&Player::getPlayerName() const
{
	return _name;
}

void	Player::updateState(const GameSession &session)
{
   if (session.getState() == GameSession::LOADING)
      _state = LOADING;
   else if (session.getState() == GameSession::PLAYING
      && _state == LOADING)
      _state = READY;
}

size_t	Player::GetScore(void) const
{
   return _score;
}
