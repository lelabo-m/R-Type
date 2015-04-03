#pragma once
#include "SFMLbutton.h"
#include "SFMLbasicText.h"

class SFMLmultigameButton : public SFMLbutton
{
protected:
	sf::Text	_desc;
	sf::Text	_nbPlayers;

public:
	SFMLmultigameButton(const sf::Vector2<int> &, const std::string &, const std::string &, const std::string &);
	virtual ~SFMLmultigameButton();

public:
	virtual void Draw(sf::RenderWindow &);
	virtual void setPosition(const sf::Rect<int> &);

private:
	void	hover(SFMLshape *);
	void	unhover(SFMLshape *);
	void	click(SFMLshape *);
	void	outTimeLobbyAnswer();
};