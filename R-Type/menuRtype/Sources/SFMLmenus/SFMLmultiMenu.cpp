#include <iostream>
#include <algorithm>
#include "SFMLmultiMenu.h"
#include "databaseSFML.h"
#include "SFMLbigButton.h"
#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "SFMLmapselectorButton.h"
#include "SFMLmultimapSelectorMenu.h"
#include "SFMLbasicInput.h"
#include "SFMLsubmenuButton.h"
#include "SFMLmonsterButton.h"
#include "SFMLmultigameButton.h"
#include "SFMLmultiplayerLobbyMenu.h"
#include "SFMLlengthlevelPopUp.h"
#include "gameOptions.h"
#include "InputPopupValue.h"

SFMLmultiMenu::SFMLmultiMenu() : _timer(3, std::bind(&SFMLmultiMenu::updateGames, this))
{
	this->_menuName = "Multi";
	this->initBasemultiMenu();
	std::cout << "Asking for games" << std::endl;
	_client->askAvailableGames();
}

SFMLmultiMenu::~SFMLmultiMenu()
{
	delete	_mapselect;
	delete	_cancel;
	delete 	_gamelisting;
}

void SFMLmultiMenu::initMultiMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	SFMLbasicText	title			= SFMLbasicText("Multiplayer setup", sf::Vector2<int>(506, 55));

	_mapselect		= new SFMLsubmenuButton("Create a game", sf::Vector2<int>(550, 580));
	_refresh		= new SFMLsubmenuButton("Refresh", sf::Vector2<int>(850, 580));
	_gamelisting	= new SFMLdropdownMultiplayer(	db->getTexture("dropdownMultiUp"), sf::Vector2<int>(165, 100),
													db->getTexture("dropdownMultiDown"), sf::Vector2<int>(165, 625),
													sf::Vector2<int>(165, 125), 10);

	_mapselect->_funcRelease = std::bind(&SFMLmultiMenu::createMapMulti, this, _mapselect);

	_gamelisting->registerTo(_eventManager, _forms);
	_refresh->_funcRelease = std::bind(&SFMLmultiMenu::Refresh, this, _refresh);

	this->_texts.push_back(title);

	this->_forms.push_back(_mapselect);
	this->_eventManager.add(_mapselect);
	this->_forms.push_back(_refresh);
	this->_eventManager.add(_refresh);
}

void SFMLmultiMenu::initBasemultiMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_cancel 	= new SFMLbigButton("Cancel", sf::Vector2<int>(350, 762));

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	_cancel->_funcRelease = std::bind(&SFMLmultiMenu::Cancel, this, _cancel);
	this->_images.push_back(backgroundSubmenu);

	this->initMultiMenu();

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void	SFMLmultiMenu::outTimeCreateGame()
{
	throw std::runtime_error("Request for creating game is not responding");
}

void	SFMLmultiMenu::createMapMulti(SFMLshape *button)
{
	SFMLInputPopUp	popUp("R-Type multiplayer", "Define the name of the game", 30);
	InputPopupValue	*InputPopup = InputPopupValue::getInst();
	gameOptions	*opts = gameOptions::getGameOptions();
	SFMLbuttonevents::submenuButtonUnhover(button);

	_menuManager->goTo("InputPopUp");
	popUp.launch();

	std::string level(InputPopup->getValue());
	if (level != "")
	{
		std::replace(level.begin(), level.end(), ',', ' ');
		std::replace(level.begin(), level.end(), ';', ' ');

		TickTimer	_time(5, std::bind(&SFMLmultiMenu::outTimeCreateGame, this));
		try
		{
			_client->askCreateGame(level, opts->getNickName());
			_client->CurrentLobby = new Lobby(level, "UNDEFINED", opts->getNickName());

			while (_client->CurrentLobby && _client->CurrentLobby->isConfirmed() == false)
			{
				_time.update();
				_client->handleNetwork();
			}
			std::cout << _client->CurrentLobby << std::endl;
			if (_client->CurrentLobby)
			{
				SFMLmultiplayerLobbyMenu	lobbyMenu(HOSTER);

				_menuManager->goTo("MultiplayerLobby");
				lobbyMenu.launch();
			}
		}
		catch (const std::runtime_error &)
		{
		}

		if (_client->CurrentLobby)
			delete _client->CurrentLobby;
	}
}

void	SFMLmultiMenu::updateGames()
{
	if (this->_gamelisting->getList().empty())
		for (auto it : _client->AvailableGames)
			this->_gamelisting->Add(new SFMLmultigameButton(sf::Vector2<int>(), it.GameName, it.MapName, std::to_string(it.PlayerAmount)));
}

void SFMLmultiMenu::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
	updateGames();
}

void SFMLmultiMenu::updateDynamicContents(const sf::Event &ev)
{


	this->_eventManager.execute(ev, this->_gamelisting->getList(), 10);
}

void SFMLmultiMenu::displayDynaicContents()
{
	this->_gamelisting->Draw(*(this->_window));

}

void	SFMLmultiMenu::Cancel(SFMLshape *)
{
	this->_menuManager->goTo("Play");
}

void	SFMLmultiMenu::Refresh(SFMLshape *)
{
	_gamelisting->Clear();
	_client->askAvailableGames();
}
