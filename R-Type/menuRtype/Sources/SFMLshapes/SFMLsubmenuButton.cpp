#include "SFMLsubmenuButton.h"
#include "SFMLbuttonevents.h"
#include "databaseSFML.h"

SFMLsubmenuButton::SFMLsubmenuButton(const std::string &txt, const sf::Vector2<int> &pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("submenuButton"));
	this->_background.setPosition((float)pos.x, (float)pos.y);

	this->_text.setString(txt);
	this->_text.setColor(sf::Color(255, 255, 255, 255));
	this->_text.setStyle(sf::Text::Italic);
	this->_text.setCharacterSize(23);
	this->_text.setFont(db->getFont("Brawler"));

	this->_area = sf::Rect<int>(pos.x, pos.y, 210, 60);

	this->InitFunctions();
	this->InitButton();

	this->_funcFocus = std::bind(SFMLbuttonevents::submenuButtonHover, this);
	this->_funcUnfocus = std::bind(SFMLbuttonevents::submenuButtonUnhover, this);
	this->_funcClick = std::bind(SFMLbuttonevents::submenuButtonClick, this);
}


SFMLsubmenuButton::~SFMLsubmenuButton()
{
}
