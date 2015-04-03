#pragma once
#include "SFMLinput.h"
class SFMLbasicInput : public SFMLinput
{
public:
	SFMLbasicInput(int, const sf::Vector2<int> &, const std::string &val="");
	virtual ~SFMLbasicInput();
};

