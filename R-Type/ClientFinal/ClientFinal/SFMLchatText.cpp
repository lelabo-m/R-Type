#include "SFMLchatText.h"
#include "SFMLbasicText.h"

SFMLchatText::SFMLchatText(const std::string &message, const sf::Texture &texture, const sf::Vector2<int> &pos) : SFMLshape(texture, pos)
{
	this->_message = SFMLbasicText(message, sf::Vector2<int>(pos.x + 5, pos.y + 3), 12);
	this->_area = sf::Rect<int>(pos.x, pos.y, 100, 20);
}

SFMLchatText::~SFMLchatText()
{
}

void	SFMLchatText::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_message);
}

void	SFMLchatText::setPosition(const sf::Rect<int> &area)
{
	this->_area = area;
	this->_background.setPosition(static_cast<float>(_area.left), static_cast<float>(_area.top));
	this->_message.setPosition(static_cast<float>(_area.left + 5), static_cast<float>(_area.top + 3));
}

const std::string SFMLchatText::getMessage() const
{
	return this->_message.getString().toAnsiString();
}