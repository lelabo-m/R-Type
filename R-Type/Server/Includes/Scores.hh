//
// Scores.hh for rtype in /mnt/hgfs/R-Type/Server
// 
// Made by Clément Roche
// Login   <roched@epitech.net>
// 
// Started on  Fri Jan  2 15:09:03 2015 Clément Roche
// Last update Sat Jan  3 11:18:42 2015 Clément Roche
//

#ifndef SCORES_HH_
# define SCORES_HH_

#include <string>
#include <map>
#include "tinyxml2.h"

class Scores
{
public:
   typedef std::map<std::string, int>	scoremap;
private:
   tinyxml2::XMLDocument		*_doc;
   std::map<std::string, scoremap>	_scores;

protected:
   void					parse(void);

public:
   explicit Scores(void);
   virtual ~Scores(void);

   bool					load(std::string &filename);
   void					save(std::string &filename);
   const std::map<std::string, int>	&getScore(const std::string mapName);
   void					setScore(const std::string mapName, const std::string player, int score);
};

#endif
