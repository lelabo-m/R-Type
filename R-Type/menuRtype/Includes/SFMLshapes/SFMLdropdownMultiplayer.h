#pragma once

#include "SFMLdropdown.h"

class SFMLdropdownMultiplayer : public SFMLdropdown
{

public:
	SFMLdropdownMultiplayer(const sf::Texture &, const sf::Vector2<int> &, const sf::Texture &, const sf::Vector2<int> &, const sf::Vector2<int> &, int nbVisible = 0);
	virtual ~SFMLdropdownMultiplayer();

private:
	void	initDropdownMulti();
	void	upHover(SFMLshape *);
	void	upUnhover(SFMLshape *);
	void	downHover(SFMLshape *);
	void	downUnhover(SFMLshape *);
};

