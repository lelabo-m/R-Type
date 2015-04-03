#include "SFMLbasePopUp.h"
#include "databaseSFML.h"

SFMLbasePopUp::SFMLbasePopUp()
{
	this->_menuName = "BasePopUp";
	this->initBasePopUp();
}

SFMLbasePopUp::~SFMLbasePopUp()
{
}

void	SFMLbasePopUp::initBasePopUp()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sf::Sprite	background(db->getTexture("backgroundLauncher"));
	sf::Sprite	backgroundPopUp(db->getTexture("popupLauncher"));

	backgroundPopUp.setPosition(sf::Vector2<float>(390, 55));

	this->_images.push_back(background);
	this->_images.push_back(backgroundPopUp);
}
