#include "SFMLmultimapSelectorMenu.h"

#include "SFMLmapselectorButton.h"
#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "databaseSFML.h"

SFMLmultimapSelectorMenu::SFMLmultimapSelectorMenu()
{
	this->_menuName = "MultimapSelector";
	this->initBasemultiMapselector();
}

SFMLmultimapSelectorMenu::~SFMLmultimapSelectorMenu()
{
	delete 	_cancel;
}

void SFMLmultimapSelectorMenu::initMultiMapselector()
{
	SFMLbasicText			title = SFMLbasicText("Select your map", sf::Vector2<int>(523, 55));
	SFMLmapselectorButton	*level = new SFMLmapselectorButton("Level 1", sf::Vector2<int>(166, 99));

	this->_texts.push_back(title);

	level->_funcRelease = std::bind(&SFMLbuttonevents::submitLevel, level);

	this->_forms.push_back(level);
	this->_eventManager.add(level);

	SFMLmapselectorButton	*level2 = new SFMLmapselectorButton("Level 2", sf::Vector2<int>(416, 99));

	level2->_funcRelease = std::bind(&SFMLbuttonevents::submitLevel, level2);

	this->_forms.push_back(level2);
	this->_eventManager.add(level2);
}

void	SFMLmultimapSelectorMenu::initBasemultiMapselector()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	_cancel = new SFMLbigButton("Return", sf::Vector2<int>(350, 762));
	_cancel->_funcRelease = std::bind(&SFMLbuttonevents::optionCancel, _cancel);

	this->_images.push_back(backgroundSubmenu);
	this->initMultiMapselector();

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}
