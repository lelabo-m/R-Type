//
// CommandLine.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Mon Dec  8 18:10:47 2014 Clément Roche
// Last update Mon Dec  8 18:32:50 2014 Clément Roche
//

#ifndef COMMANDLINE_HH_
# define COMMANDLINE_HH_

#include <string>
#include <stack>
#include <mutex>
#include <thread>

class CommandLine
{
private:
   std::stack<std::string>	_stack;
   std::mutex			_mutex;
   bool				_run;
   std::thread			_thread;

protected:
   void		run(void);
   void		append(std::string &str);

public:
   CommandLine(void);
   ~CommandLine(void);

   std::string	getLastCommand(void);
   unsigned int	getCommandCount(void) const;
};

#endif
