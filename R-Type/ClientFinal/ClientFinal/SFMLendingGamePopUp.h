#pragma once
#include "SFMLbasePopUp.h"
#include "SFMLbasicText.h"
#include "SFMLbuttonLauncher.h"

class SFMLendingGamePopUp : public SFMLbasePopUp
{
private:
	SFMLbuttonLauncher		*_quit;
	sf::Text				_title;
	sf::Text				_scoreText;

public:
	SFMLendingGamePopUp();
	virtual ~SFMLendingGamePopUp();

private:
	void	Quit(SFMLshape *);
};

