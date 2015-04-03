#include "SFMLmapscoreSelectorMenu.h"
#include "SFMLmapselectorButton.h"

#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "databaseSFML.h"

SFMLmapscoreSelectorMenu::SFMLmapscoreSelectorMenu()
{

	this->_menuName = "MapscoreSelector";
	this->initBaseMapscoreselectorMenu();
	_client->askAvailableMaps();
}


SFMLmapscoreSelectorMenu::~SFMLmapscoreSelectorMenu()
{
	delete _cancel;
}

void	SFMLmapscoreSelectorMenu::createLevelButtons()
{
	SFMLbasicText			title = SFMLbasicText("Select your map", sf::Vector2<int>(523, 55));

	this->_texts.push_back(title);
}

void	SFMLmapscoreSelectorMenu::initBaseMapscoreselectorMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_cancel = new SFMLbigButton("Return", sf::Vector2<int>(350, 762));

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	_cancel->_funcRelease = std::bind(&SFMLbuttonevents::optionCancel, _cancel);
	this->_images.push_back(backgroundSubmenu);

	this->createLevelButtons();

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void SFMLmapscoreSelectorMenu::updateDynamicContents(const sf::Event &ev)
{
	int	i = 0;

	_listMaps.clear();
	for (const auto &map : _client->AvailableMaps)
	{
		this->_listMaps.push_back(new SFMLmapselectorButton(map, sf::Vector2<int>(166 + ((i % 4) * 250) , 99 + ((i / 4) * 140))));
		++i;
	}
	_eventManager.execute(ev, _listMaps, 16);
}

void SFMLmapscoreSelectorMenu::displayDynaicContents()
{
	for (const auto &map : this->_listMaps)
	{
		map->Draw(*(this->_window));
	}
}
