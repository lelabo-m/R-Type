//
// GamePackage.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Dec 10 13:14:19 2014 Clément Roche
// Last update Wed Dec 10 13:39:40 2014 Clément Roche
//

#ifndef GAMEPACKAGE_HH_
# define GAMEPACKAGE_HH_

#include <list>
#include <string>
#include "Client.hh"

class GamePackage
{
private:
   std::list<std::string>	_files;
   std::string			_levelFilename;

public:
   GamePackage(void);
   ~GamePackage(void);

   bool	sendToClient(Client *client);
};

#endif
