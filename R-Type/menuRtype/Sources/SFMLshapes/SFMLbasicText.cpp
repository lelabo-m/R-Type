#include "SFMLbasicText.h"
#include "databaseSFML.h"

SFMLbasicText::SFMLbasicText()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->setFont(db->getFont("Brawler"));
	this->setStyle(sf::Text::Italic);
}

SFMLbasicText::SFMLbasicText(const std::string &txt, const sf::Vector2<int> &pos, int size)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->setString(txt);
	this->setPosition((float)pos.x, (float)pos.y);
	this->setFont(db->getFont("Brawler"));
	this->setCharacterSize(size);
	this->setStyle(sf::Text::Italic);
}

SFMLbasicText::~SFMLbasicText()
{
}

bool	SFMLbasicText::operator==(const SFMLbasicText &oth)
{
	return this->getString() == oth.getString();
}
