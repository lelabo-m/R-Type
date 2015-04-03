#pragma once

#include "SFMLslider.h"

class SFMLbasicSlider : public SFMLslider
{
public:
	SFMLbasicSlider(int, int, int, const sf::Vector2<int> &pos);
	virtual ~SFMLbasicSlider();
};