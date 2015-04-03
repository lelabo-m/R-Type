//
// TCPHandler.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Sun Dec 14 18:59:04 2014 Clément Roche
// Last update Sun Dec 14 20:41:22 2014 Clément Roche
//

#include "TCPHandler.hh"

TCPHandler::TCPHandler(int procedureCode, std::string name, std::function<bool(Server &, Client *, TCPFrame *)> function) : _function(function), _procedureCode(procedureCode), _name(name)
{
}

TCPHandler::~TCPHandler(void)
{
}

int	TCPHandler::getProcedureCode(void) const
{
   return _procedureCode;
}

std::string	TCPHandler::getName(void) const
{
   return _name;
}

bool	TCPHandler::operator()(Server &server, Client *client, TCPFrame *frame)
{
   return _function(server, client, frame);
}
