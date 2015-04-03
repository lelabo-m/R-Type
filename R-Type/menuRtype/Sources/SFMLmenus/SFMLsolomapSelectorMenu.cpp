#include "SFMLsolomapSelectorMenu.h"
#include "SFMLbigButton.h"
#include "SFMLmapselectorButton.h"
#include "SFMLbuttonevents.h"
#include "databaseSFML.h"

SFMLsolomapSelectorMenu::SFMLsolomapSelectorMenu()
{
	this->_menuName = "SolomapSelector";
	this->initSoloMenu();
	_client->askAvailableMaps();
}

SFMLsolomapSelectorMenu::~SFMLsolomapSelectorMenu()
{
	delete 	_cancel;
}

void	SFMLsolomapSelectorMenu::initMapselector()
{
	SFMLmapselectorButton	*level = new SFMLmapselectorButton("Level 1", sf::Vector2<int>(166, 99));
	SFMLmapselectorButton	*level2 = new SFMLmapselectorButton("Level 2", sf::Vector2<int>(416, 99));

	this->_forms.push_back(level);
	this->_eventManager.add(level);

	this->_forms.push_back(level2);
	this->_eventManager.add(level2);
}

void	SFMLsolomapSelectorMenu::initSoloMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_cancel = new SFMLbigButton("Cancel", sf::Vector2<int>(350, 762));
	_cancel->_funcRelease = std::bind(&SFMLbuttonevents::optionCancel, _cancel);

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	this->_images.push_back(backgroundSubmenu);

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);

	this->initMapselector();
}
