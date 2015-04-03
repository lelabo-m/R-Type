#include "SFMLplaymapSelectorMenu.h"
#include "databaseSFML.h"
#include "SFMLmapselectorButton.h"
#include "SFMLbigButton.h"
#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "MapSelected.h"
#include "FileSystem.hpp"

SFMLplaymapSelectorMenu::SFMLplaymapSelectorMenu()
{
	this->_menuName = "Mapselector";
	this->initBasemapSelectorMenu();
	_client->askAvailableMaps();
}

SFMLplaymapSelectorMenu::~SFMLplaymapSelectorMenu()
{
	delete 	_cancel;
	delete	_refresh;
}

void SFMLplaymapSelectorMenu::initMapSelector()
{
	SFMLbasicText			title = SFMLbasicText("Select your map", sf::Vector2<int>(523, 55));

	this->_texts.push_back(title);
}

void SFMLplaymapSelectorMenu::initBasemapSelectorMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_cancel = new SFMLbigButton("Return", sf::Vector2<int>(150, 762));
	_refresh = new SFMLbigButton("Refresh", sf::Vector2<int>(450, 762));
	_cancel->_funcRelease = std::bind(&SFMLplaymapSelectorMenu::Cancel, this, _cancel);
	_refresh->_funcRelease = std::bind(&SFMLplaymapSelectorMenu::Refresh, this, _refresh);

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	this->_images.push_back(backgroundSubmenu);

	this->initMapSelector();

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);

	this->_forms.push_back(_refresh);
	this->_eventManager.add(_refresh);
}

void SFMLplaymapSelectorMenu::executeRequests()
{
	int	i = 0;

	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
	if (_listMaps.empty())
	{
		for (const auto &map : _client->AvailableMaps)
		{
			SFMLmapselectorButton *level = new SFMLmapselectorButton(FileSystem::FileShortName(map), sf::Vector2<int>(166 + ((i % 4) * 250), 99 + ((i / 4) * 140)));
			level->_funcRelease = std::bind(&SFMLplaymapSelectorMenu::SubmitLevel, this, level);

			this->_listMaps.push_back(level);
			++i;
		}
	}
}

void SFMLplaymapSelectorMenu::updateDynamicContents(const sf::Event &ev)
{
	_eventManager.execute(ev, _listMaps, 16);
}

void SFMLplaymapSelectorMenu::displayDynaicContents()
{
	for (const auto &map : this->_listMaps)
	{
		map->Draw(*(this->_window));
	}
}

void	SFMLplaymapSelectorMenu::Cancel(SFMLshape *)
{
	MapSelected		*mapSelected = MapSelected::getInst();

	mapSelected->setMap("");
	this->_menuManager->goToPrevious();
}

void	SFMLplaymapSelectorMenu::SubmitLevel(SFMLshape *button)
{
	SFMLbutton		*buttonPtr;
	MapSelected		*mapSelected = MapSelected::getInst();

	if ((buttonPtr = dynamic_cast<SFMLbutton *>(button)))
	{
		mapSelected->setMap(buttonPtr->_text.getString().toAnsiString());
		this->_menuManager->goToPrevious();
	}
}

void	SFMLplaymapSelectorMenu::Refresh(SFMLshape *)
{
	_listMaps.clear();
	_client->askAvailableMaps();
}