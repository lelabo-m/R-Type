//
// TCPHandler.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Sun Dec 14 18:52:13 2014 Clément Roche
// Last update Mon Dec 15 19:39:02 2014 Clément Roche
//

#ifndef TCPHANDLER_HH_
# define TCPHANDLER_HH_

#include <functional>
#include <string>
#include "TCPFrame.h"

#define BIND(X)	std::bind((X), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

class Server;
class Client;

class TCPHandler
{
private:
   std::function<bool(Server &, Client *, TCPFrame *)>	_function;
   int							_procedureCode;
   std::string						_name;

public:
   explicit TCPHandler(int procedureCode, std::string name, std::function<bool(Server &, Client *, TCPFrame *)> function);
   virtual ~TCPHandler(void);

   int		getProcedureCode(void) const;
   std::string  getName(void) const;
   bool		operator()(Server &, Client *client, TCPFrame *);
};

#endif
