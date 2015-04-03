#include "SFMLplayMenu.h"
#include "SFMLbuttonevents.h"
#include "gameOptions.h"
#include "databaseSFML.h"
#include "MapSelected.h"
#include "SFMLplaymapSelectorMenu.h"
#include "SFMLmultiplayerLobbyMenu.h"
#include "SFMLmultiMenu.h"
#include "SFMLstartEditorPopUp.h"
#include "RTypeGame.h"

SFMLplayMenu::SFMLplayMenu()
{
	this->_menuName = "Play";
	this->initPlayMenu();
}

SFMLplayMenu::~SFMLplayMenu()
{
	delete 	_single;
	delete 	_multi;
	delete 	_editor;
	delete	_cancel;
}

void	SFMLplayMenu::initPlayMenu()
{
	_single 	= new SFMLlittleButton("Single", sf::Vector2<int>(60, 762));
	_multi 		= new SFMLlittleButton("Multi", sf::Vector2<int>(252, 762));
	_editor 	= new SFMLlittleButton("Editor", sf::Vector2<int>(440, 762));
	_cancel 	= new SFMLlittleButton("Cancel", sf::Vector2<int>(630, 762));

	_cancel->_funcRelease	= std::bind(&SFMLplayMenu::Cancel, this, _cancel);
	_single->_funcRelease	= std::bind(&SFMLplayMenu::Single, this, _single);
	_multi->_funcRelease	= std::bind(&SFMLplayMenu::Multi, this, _multi);
	_editor->_funcRelease	= std::bind(&SFMLplayMenu::Editor, this, _editor);

	this->_eventManager.add(_single);
	this->_eventManager.add(_multi);
	this->_eventManager.add(_editor);
	this->_eventManager.add(_cancel);

	this->_forms.push_back(_single);
	this->_forms.push_back(_multi);
	this->_forms.push_back(_editor);
	this->_forms.push_back(_cancel);
}

void	SFMLplayMenu::outTimeCreateGame()
{
	throw std::runtime_error("Request for creating game is not responding");
}

void	SFMLplayMenu::Single(SFMLshape *)
{
	gameOptions	*opts = gameOptions::getGameOptions();
	try
	{
		_client->askCreateGame("Solo game", opts->getNickName(), true);
		_client->CurrentLobby = new Lobby("Solo game", "UNDEFINED", opts->getNickName());

		TickTimer	_time(5, std::bind(&SFMLplayMenu::outTimeCreateGame, this));
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
}

void	SFMLplayMenu::Multi(SFMLshape *)
{
	SFMLmultiMenu	multiMenu;

	_menuManager->goTo("Multi");
	multiMenu.launch();
}

void	SFMLplayMenu::Editor(SFMLshape *)
{
	SFMLstartEditorPopUp	popUp;

	_menuManager->goTo("StartEditorPopUp");
	popUp.launch();
}

void	SFMLplayMenu::Cancel(SFMLshape *)
{
	_menuManager->goTo("Main");
}