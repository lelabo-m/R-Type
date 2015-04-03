#pragma once
#include "SFMLshape.h"
#include "SFMLbutton.h"
#include "SFMLbasicText.h"

enum playerStatus
{
	HOSTER,
	GUEST
};

class SFMLplayermulti :
	public SFMLshape
{
public:
	SFMLbutton	*	_action;
	sf::Text		_pseudo;

public:
	SFMLplayermulti(const sf::Vector2<int> &, const std::string &, const sf::Texture &, playerStatus);
	virtual ~SFMLplayermulti();

public:
	virtual void Draw(sf::RenderWindow &);
	virtual void setPosition(const sf::Rect<int> &);
};

