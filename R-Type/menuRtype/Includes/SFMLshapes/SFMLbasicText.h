#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class SFMLbasicText : public sf::Text
{
public:
	SFMLbasicText();
	SFMLbasicText(const std::string &, const sf::Vector2<int> &, int size = 30);
	virtual ~SFMLbasicText();
	bool	operator==(const SFMLbasicText &);
};