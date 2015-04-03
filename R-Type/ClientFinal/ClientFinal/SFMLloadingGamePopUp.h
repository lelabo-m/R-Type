#pragma once
#include "SFMLbasePopUp.h"
#include "ClientManager.h"

class SFMLloadingGamePopUp : public SFMLbasePopUp
{
private:
	sf::Text	_text;
	std::map<Game::GameStatus, std::string>	_mapStatus;

public:
	SFMLloadingGamePopUp(const std::string &);
	virtual ~SFMLloadingGamePopUp();

private:
	void	executeRequests();
	void	displayDynaicContents();
};

