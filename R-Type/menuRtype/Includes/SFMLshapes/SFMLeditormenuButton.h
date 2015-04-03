#pragma once
#include "SFMLbutton.h"
class SFMLeditormenuButton : public SFMLbutton
{
protected:
	SFMLeditormenuButton();

public:
	SFMLeditormenuButton(const std::string &, const sf::Vector2<int> &);
	virtual ~SFMLeditormenuButton();
};

