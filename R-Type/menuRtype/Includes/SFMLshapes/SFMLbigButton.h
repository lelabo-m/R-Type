#pragma once

#include "SFMLbutton.h"

class SFMLbigButton : public SFMLbutton
{
public:
	SFMLbigButton(const std::string &, const sf::Vector2<int> &);
	virtual ~SFMLbigButton();
};

