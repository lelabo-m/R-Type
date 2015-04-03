#include "databaseSFML.h"
#include "SFMLbuttonevents.h"
#include "SFMLmapselectorButton.h"

SFMLmapselectorButton::SFMLmapselectorButton(const std::string &txt, const sf::Vector2<int> &pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("mapselectorButton"));
	this->_background.setPosition((float)pos.x, (float)pos.y);

	this->_text.setString(txt);
	this->_text.setColor(sf::Color(255, 255, 255, 255));
	this->_text.setStyle(sf::Text::Italic);
	this->_text.setCharacterSize(30);
	this->_text.setFont(db->getFont("Brawler"));

	this->_area = sf::Rect<int>(pos.x, pos.y, 200, 120);

	this->InitFunctions();
	this->InitButton();

	this->_funcFocus = std::bind(SFMLbuttonevents::mapselectorButtonHover, this);
	this->_funcUnfocus = std::bind(SFMLbuttonevents::mapselectorButtonUnhover, this);
	this->_funcClick = std::bind(SFMLbuttonevents::mapselectorButtonClick, this);
}


SFMLmapselectorButton::~SFMLmapselectorButton()
{
}
