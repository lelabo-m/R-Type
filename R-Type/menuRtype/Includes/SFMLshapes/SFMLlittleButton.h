#pragma once
#include "SFMLbutton.h"

class SFMLlittleButton : public SFMLbutton
{
public:
	SFMLlittleButton(const std::string &, const sf::Vector2<int> &);
	virtual ~SFMLlittleButton();
};

