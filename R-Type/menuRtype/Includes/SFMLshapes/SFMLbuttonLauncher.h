#pragma once
#include "SFMLbutton.h"

class SFMLbuttonLauncher :
	public SFMLbutton
{
public:
	SFMLbuttonLauncher(const sf::Vector2<int> &, const std::string &);
	virtual ~SFMLbuttonLauncher();

private:
	void	hoverFunc(SFMLshape *);
	void	unhoverFunc(SFMLshape *);
};

