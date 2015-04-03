#include "SFMLbuttonLauncher.h"
#include "databaseSFML.h"
#include "SFMLbasicText.h"

SFMLbuttonLauncher::SFMLbuttonLauncher(const sf::Vector2<int> &pos, const std::string &txt)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("popupButtonBackground"));
	this->_background.setPosition((float)pos.x, (float)pos.y);
	this->_area = sf::Rect<int>(pos.x, pos.y, (int)_background.getLocalBounds().width, (int)_background.getLocalBounds().height);
	this->_text = SFMLbasicText(txt, sf::Vector2<int>(0, 0), 20);
	this->_text.setColor(sf::Color(166, 166, 166, 255));

	this->InitButton();
	this->_funcFocus = std::bind(&SFMLbuttonLauncher::hoverFunc, this, this);
	this->_funcUnfocus = std::bind(&SFMLbuttonLauncher::unhoverFunc, this, this);
}


SFMLbuttonLauncher::~SFMLbuttonLauncher()
{
}

void	SFMLbuttonLauncher::hoverFunc(SFMLshape *)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_text.setColor(sf::Color(255, 255, 255, 255));
	this->_background.setTexture(db->getTexture("popupButtonBackgroundHover"));
}

void	SFMLbuttonLauncher::unhoverFunc(SFMLshape *)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_text.setColor(sf::Color(166, 166, 166, 255));
	this->_background.setTexture(db->getTexture("popupButtonBackground"));
}
