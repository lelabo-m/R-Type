#pragma once
#include <iostream>
#include "SFMLbutton.h"

class SFMLmonsterButton : public SFMLbutton
{
protected:
	sf::Sprite		_monsterSprite;

public:
	SFMLmonsterButton(const sf::Vector2<int> &, const sf::Texture &);
	virtual ~SFMLmonsterButton();

public:
	virtual void Draw(sf::RenderWindow &);
	void	down();
	void	up();
	const sf::Sprite &getMonsterSprite() const;

public:
	virtual void				setPosition(const sf::Rect<int> &);
};

std::ostream	&operator<<(std::ostream &, const SFMLmonsterButton &);