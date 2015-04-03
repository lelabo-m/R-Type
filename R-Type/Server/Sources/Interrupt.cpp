//
// Interrupt.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Mon Dec  8 12:00:00 2014 Clément Roche
// Last update Mon Dec  8 12:06:21 2014 Clément Roche
//

#include <signal.h>
#include "Interrupt.hh"

static Server	*glServer(Server *ptr = 0)
{
   static Server *_ptr = 0;

   if (ptr)
      _ptr = ptr;
   return _ptr;
}

static void	handler(int sig)
{
   (void) sig;
   Server *s = glServer();
   if (s)
      s->stop();
}

void	setInterrupt(Server *ptr)
{
   glServer(ptr);
   signal(SIGINT, handler);
}
