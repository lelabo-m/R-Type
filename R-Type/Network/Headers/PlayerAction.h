#ifndef _PLAYERACTION_H_
# define _PLAYERACTION_H_

#include <string>
#include "GameMessage.h"

class PlayerAction
{
public:
	static PlayerAction *unserialize(const std::string &data)
	{
		return new PlayerAction(data[0]);
	}

	static void serialize(std::string &str, char action)
	{
		str = std::string(GAME_MESSAGE_CONTENT_LENGTH, 0);
		str[0] = action;
	}

	~PlayerAction();
	char getAction() const;
	void setAction(char action);
protected:
	PlayerAction(char action);
protected:
	char _action;
};

#endif