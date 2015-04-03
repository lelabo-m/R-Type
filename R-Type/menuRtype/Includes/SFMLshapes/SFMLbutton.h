#pragma once

#include <string>
#include <functional>
#include "SFMLshape.h"

class SFMLbutton : public SFMLshape
{

public:
	sf::Text		_text;

protected:
	SFMLbutton();

public:
	SFMLbutton(const std::string &, const sf::Texture &, const sf::Rect<int> &);
	SFMLbutton(const sf::Text &, const sf::Texture &, const sf::Rect<int> &);
	SFMLbutton(const sf::Text &, const sf::Texture &, const sf::Vector2<int> &);
	SFMLbutton(const std::string &, const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLbutton();
	const SFMLbutton &operator=(const SFMLbutton &);
	const SFMLbutton &operator=(const SFMLbutton &&);

public:
	virtual void Draw(sf::RenderWindow &window);

protected:
	virtual void InitButton();
};

