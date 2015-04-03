//
// CommandLine.cpp for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Mon Dec  8 18:13:11 2014 Clément Roche
// Last update Wed Dec 10 12:04:34 2014 Clément Roche
//

#include <iostream>
#include "CommandLine.hh"

CommandLine::CommandLine(void) : _run(true), _thread(&CommandLine::run, this)
{
}

CommandLine::~CommandLine(void)
{
   _run = false;
   _thread.join();
}

std::string	CommandLine::getLastCommand(void)
{
   std::lock_guard<std::mutex> lock(_mutex);

   if (getCommandCount())
   {
      std::string ret(_stack.top());
      _stack.pop();
      return (ret);
   }
   return ("");
}

unsigned int	CommandLine::getCommandCount(void) const
{
   return _stack.size();
}

void	CommandLine::run(void)
{
   while (_run)
   {
      char raw[256];
      std::cin.getline(raw, 255);
      std::string input(raw);
      append(input);
   }
}

void	CommandLine::append(std::string &str)
{
   std::lock_guard<std::mutex> lock(_mutex);
   _stack.push(str);
}
