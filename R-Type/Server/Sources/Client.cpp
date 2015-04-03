//
// Client.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Nov 26 20:20:28 2014 Clément Roche
// Last update Sat Dec 20 19:11:47 2014 Clément Roche
//

#include <iostream>
#include "GameSession.hh"
#include "TCPMessageFactoryServer.h"
#include "Client.hh"

Client::Client(std::shared_ptr<Session> &session) : _id(0), _session(session), _currentGame(0)
{
   _id = session->getSessionID();
}

Client::~Client(void)
{
   if (_currentGame)
      std::cerr << "\tWARNING A CLIENT WAS KILLED BUT STILL CONNECTED TO A GAME !" << std::endl;
   std::cout << "CLIENT DESTRUCTOR ! " << std::endl;
}

unsigned int	Client::getID(void) const
{
   return _id;
}

bool	Client::isConnected(void) const
{
   if (_session && _session->getSessionState() == Session::CONNECTED)
      return true;
   return false;
}

bool	Client::sendMessage(const std::string &content, Protocole::Server::Procedure procedure)
{
	if (!isConnected())
		return false;
	TCPMessageFactory *f = TCPMessageFactoryServer::getInstance();

	f->SendMsg(_session, content, procedure);
	return true;
}

void	Client::sendUDPMessage(UDPFrame *frame)
{
   if (isConnected())
       _session->pushUDPToSendFrame(frame);
}

unsigned int	Client::receivedMessageCount(void) const
{
   if (!isConnected())
      return (0);
   return (_session->countTCPReceivedFrame());
}

TCPFrame *Client::getReceivedMessage(void)
{
   if (!isConnected())
      return (0);
   return _session->getTCPReceivedFrame();
}

unsigned int	Client::receivedUDPCount(void) const
{
	if (!isConnected())
		return (0);
	return _session->countUDPReceivedFrame();
}

UDPFrame	*Client::getReceivedUDP(void)
{
	if (!isConnected())
		return (0);
	return _session->getUDPReceivedFrame();
}

void	Client::setCurrentGame(GameSession *game)
{
   _currentGame = game;
}

GameSession	*Client::getCurrentGame(void) const
{
   return _currentGame;
}

std::shared_ptr<Session> Client::getClientSession()
{
	return _session;
}
