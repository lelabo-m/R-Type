#include "SFMLbigButton.h"
#include "SFMLbuttonevents.h"
#include "databaseSFML.h"

SFMLbigButton::SFMLbigButton(const std::string &txt, const sf::Vector2<int> &pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("bigButton"));
	this->_background.setPosition((float)pos.x, (float)pos.y);

	this->_text.setString(txt);
	this->_text.setColor(sf::Color(169, 237, 255, 255));
	this->_text.setStyle(sf::Text::Italic);
	this->_text.setCharacterSize(30);
	this->_text.setFont(db->getFont("Brawler"));

	this->_area = sf::Rect<int>(pos.x, pos.y, 232, 60);

	this->InitFunctions();
	this->InitButton();

	this->_funcFocus = std::bind(SFMLbuttonevents::bigButtonHover, this);
	this->_funcUnfocus = std::bind(SFMLbuttonevents::bigButtonUnhover, this);
	this->_funcClick = std::bind(SFMLbuttonevents::bigButtonClick, this);
}


SFMLbigButton::~SFMLbigButton()
{
}
