#include "SFMLmonsterButton.h"
#include "SFMLbuttonevents.h"
#include "databaseSFML.h"

SFMLmonsterButton::SFMLmonsterButton(const sf::Vector2<int> &pos, const sf::Texture &monster)
{
	this->_monsterSprite.setTexture(monster);
	this->_area = sf::Rect<int>(pos.x, pos.y, 100, 100);

	this->_background.setPosition((float)pos.x, (float)pos.y);
	this->_monsterSprite.setPosition((float)pos.x + 18, (float)pos.y + 18);

	this->_funcFocus	= std::bind(&SFMLbuttonevents::monsterButtonHover, this);
	this->_funcUnfocus	= std::bind(&SFMLbuttonevents::monsterButtonUnhover, this);
}

SFMLmonsterButton::~SFMLmonsterButton()
{
	std::cout << "LEL" << std::endl;
}

void	SFMLmonsterButton::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_monsterSprite);
}

void	SFMLmonsterButton::up()
{
	this->_area.top -= 110;
	this->_background.setPosition((float)this->_area.left, (float)this->_area.top);
	this->_monsterSprite.setPosition((float)this->_area.left + 18, (float)this->_area.top + 18);
}

void	SFMLmonsterButton::down()
{
	this->_area.top += 110;
	this->_background.setPosition((float)this->_area.left, (float)this->_area.top);
	this->_monsterSprite.setPosition((float)this->_area.left + 18, (float)this->_area.top + 18);
}

const sf::Sprite &SFMLmonsterButton::getMonsterSprite() const
{
	return this->_monsterSprite;
}

void SFMLmonsterButton::setPosition(const sf::Rect<int> &area)
{
	this->_area = area;
	this->_background.setPosition((float)area.left, (float)area.top);
	this->_monsterSprite.setPosition((float)area.left + 18, (float)area.top + 18);
}

std::ostream	&operator<<(std::ostream &flux, const SFMLmonsterButton &button)
{
	sf::Rect<int>		area = button.getPosition();

	return flux << "Position : x = " << area.left << " ; y = " << area.top;
}
