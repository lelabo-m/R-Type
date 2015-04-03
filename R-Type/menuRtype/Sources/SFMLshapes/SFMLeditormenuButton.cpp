#include "SFMLeditormenuButton.h"
#include "SFMLbuttonevents.h"
#include "databaseSFML.h"

SFMLeditormenuButton::SFMLeditormenuButton()
{
}

SFMLeditormenuButton::SFMLeditormenuButton(const std::string &txt, const sf::Vector2<int> &pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_text.setString(txt);
	this->_text.setCharacterSize(20);
	this->_text.setFont(db->getFont("Brawler"));
	this->_text.setStyle(sf::Text::Italic);
	this->_background.setTexture(db->getTexture("menueditorButton"));
	this->_background.setPosition((float)pos.x, (float)pos.y);
	this->_area = sf::Rect<int>(pos.x, pos.y, (int)_background.getLocalBounds().width, (int)_background.getLocalBounds().height);

	this->InitButton();
	this->InitFunctions();

	this->_funcFocus	= SFMLbuttonevents::editorButtonHover;
	this->_funcUnfocus	= SFMLbuttonevents::editorButtonUnhover;
}

SFMLeditormenuButton::~SFMLeditormenuButton()
{
}
