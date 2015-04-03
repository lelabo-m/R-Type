//
// GamePackage.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Dec 10 13:16:46 2014 Clément Roche
// Last update Wed Dec 10 13:17:38 2014 Clément Roche
//

#include "GamePackage.hh"

GamePackage::GamePackage(void)
{
}

GamePackage::~GamePackage(void)
{
}

bool	GamePackage::sendToClient(Client *client)
{
   if (!client)
      return false;
   return true;
}
