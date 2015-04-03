//
// main.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Wed Nov 26 19:51:15 2014 Clément Roche
// Last update Sun Dec  7 11:42:20 2014 Clément Roche
//
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iostream>
#include <exception>
#include "Server.hh"

int	main(int, char **)
{
	std::srand((size_t)std::time(nullptr));
   Server s;

   try
   {
      s.start();
   }
   catch (std::exception &e)
   {
      std::cerr << "Server encountered an unexpected error : " << e.what() << std::endl;
   }
   return (0);
}
