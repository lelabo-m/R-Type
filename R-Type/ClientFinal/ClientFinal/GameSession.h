#ifndef _GAMESESSION_H_
# define _GAMESESSION_H_

#include <string>

class GameSession
{
public:
	GameSession(const std::string &gamename,
				const std::string &mapname,
				unsigned int pAmount);
	~GameSession();

public:
	std::string GameName;
	std::string MapName;
	unsigned int PlayerAmount;

};

#endif