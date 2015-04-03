#pragma once
#include "SFMLshape.h"

class SFMLchatText : public SFMLshape
{
public:
	sf::Text	_message;

public:
	SFMLchatText(const std::string &, const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLchatText();
	virtual void		Draw(sf::RenderWindow &);
	virtual void		setPosition(const sf::Rect<int> &);
	const std::string	getMessage() const;
};

