#include "SFMLmultiplayerLobbyMenu.h"
#include "SFMLbuttonevents.h"
#include "SFMLplaymapSelectorMenu.h"
#include "SFMLloadingGamePopUp.h"
#include "databaseSFML.h"
#include "SFMLchatText.h"
#include "MapSelected.h"

SFMLmultiplayerLobbyMenu::SFMLmultiplayerLobbyMenu(playerStatus pl)
{
	this->_menuName = "MultiplayerLobby";
	this->_status = pl;
	this->initBaseMenu();
	_launch = nullptr;
	_mapSelect = nullptr;
}

SFMLmultiplayerLobbyMenu::~SFMLmultiplayerLobbyMenu()
{
	if (_launch)
		delete 	_launch;
	if (_mapSelect)
		delete 	_mapSelect;
	delete 	_players;
	delete 	_chat;
	delete 	_cancel;
	delete	_chatInput;



	if (_client->CurrentGame)
		delete _client->CurrentGame;
	_client->CurrentGame = nullptr;
	_client->askLeaveLobby();
}

void SFMLmultiplayerLobbyMenu::initLobby()
{
	databaseSFML	*db = databaseSFML::getDatabase();
	SFMLbasicText	title("Lobby room", sf::Vector2<int>(290, 50));
	SFMLbasicText	GameName("Game name : " + _client->CurrentLobby->getGameName(), sf::Vector2<int>(165, 150), 20);

	_MapName = SFMLbasicText("Map name : " + _client->CurrentLobby->getMapName() , sf::Vector2<int>(165, 100), 20);

	if (_status == HOSTER)
	{
		_launch = new SFMLsubmenuButton("Launch", sf::Vector2<int>(150, 590));
		_mapSelect = new SFMLsubmenuButton("Select a map", sf::Vector2<int>(390, 590));

		_launch->_funcRelease = std::bind(&SFMLmultiplayerLobbyMenu::launchGame, this, _launch);
		_mapSelect->_funcRelease = std::bind(&SFMLmultiplayerLobbyMenu::defineMap, this, _mapSelect);
		this->_eventManager.add(_launch);
		this->_eventManager.add(_mapSelect);

		this->_forms.push_back(_launch);
		this->_forms.push_back(_mapSelect);
	}

	_players	= new SFMLunrotateDropdown(	db->getTexture("dropdownMultiUp"), sf::Vector2<int>(165, 200),
											db->getTexture("dropdownMultiDown"), sf::Vector2<int>(165, 525),
											sf::Vector2<int>(165, 225), 10);
	_players->registerTo(this->_eventManager, this->_forms);

	this->_texts.push_back(GameName);
	this->_texts.push_back(title);
}

void SFMLmultiplayerLobbyMenu::initChat()
{
	databaseSFML	*db = databaseSFML::getDatabase();
	sf::Sprite		backgroundChat(db->getTexture("backgroundChat"));
	SFMLbasicText	title("Chat room", sf::Vector2<int>(785,50));
	backgroundChat.setPosition(610, 90);

	_chat = new SFMLunrotateDropdown(	db->getTexture("chatArrowUp"), sf::Vector2<int>(1080, 95),
										db->getTexture("chatArrowDown"), sf::Vector2<int>(1080, 620),
										sf::Vector2<int>(620, 565), 23, LASTS);
	_chat->registerTo(this->_eventManager, this->_forms);

	_chatInput = new SFMLinputLauncher(sf::Vector2<int>(630, 600), 30);
	_chatInput->_funcValidInput = std::bind(&SFMLmultiplayerLobbyMenu::addToChat, this, _chatInput);

	this->_forms.push_back(_chatInput);
	this->_eventManager.add(_chatInput);

	this->_images.push_back(backgroundChat);
	this->_texts.push_back(title);
}

void SFMLmultiplayerLobbyMenu::initBaseMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_cancel = new SFMLbigButton("Cancel", sf::Vector2<int>(350, 762));

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	_cancel->_funcRelease = std::bind(&SFMLmultiplayerLobbyMenu::Cancel, this, _cancel);
	this->_images.push_back(backgroundSubmenu);

	this->initLobby();
	this->initChat();

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void SFMLmultiplayerLobbyMenu::updateDynamicContents(const sf::Event &)
{
}

void SFMLmultiplayerLobbyMenu::displayDynaicContents()
{
	databaseSFML *db = databaseSFML::getDatabase();
	Lobby *lobby = _client->CurrentLobby;

	if (lobby != nullptr && lobby->isConfirmed() == true)
	{
		unsigned int i = 0;
		const std::list<std::string> &list = lobby->getPlayers();

		_MapName.setString("Map : " + lobby->getMapName());
		_players->Clear();
		for (auto ite : list)
		{
			_players->Add(new SFMLplayermulti(sf::Vector2<int>(165, (30 * i) + 225), ite, db->getTexture("playerlistingBackground"), (i == 0 ? HOSTER : GUEST)));
			i++;
		}

		i = 0;
		_chat->Clear();
		for (auto &ite : lobby->Messages)
		{
			_chat->Add(new SFMLchatText(ite.substr(0, ite.size() - 2), db->getTexture("void"), sf::Vector2<int>(620, (20 * i) + 125)));
			i++;
		}
	}

	this->_chat->Draw(*(this->_window));
	this->_players->Draw(*(this->_window));
	this->_window->draw(this->_MapName);
}

void 	SFMLmultiplayerLobbyMenu::defineMap(SFMLshape *)
{
	SFMLplaymapSelectorMenu 	mapSelector;
	MapSelected					*mapSelected = MapSelected::getInst();

	_menuManager->goTo("Mapselector");
	mapSelector.launch();
	std::string 	levelName(mapSelected->getMap());

	if (levelName != "")
	{
		_MapName.setString("Map : " + levelName);
		_client->CurrentLobby->setMapName(levelName);
		_client->askChangeMap(levelName + ".map");
	}
}

void	SFMLmultiplayerLobbyMenu::addToChat(SFMLinput *input)
{
	if (input->getMessage() != "")
	{
		this->_client->askSendLobbyMessage(input->getMessage());
		input->setMessage("");

		input->_textingFocus = true;
	}
}

void	SFMLmultiplayerLobbyMenu::Cancel(SFMLshape *)
{
	_client->askLeaveLobby();
	_menuManager->goTo("Play");
}

void	SFMLmultiplayerLobbyMenu::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_client->askLeaveLobby();
		this->_menuManager->goTo("Launcher");
	}
	if (!_client->CurrentLobby)
	{
		this->_client->CurrentLobby = nullptr;
		this->_menuManager->goTo("Multi");
	}
	if (_client->CurrentGame)
	{
		SFMLloadingGamePopUp	popUp("Loadding...");

		_menuManager->goTo("LoadingPopUp");
		popUp.launch();
	}
}

void	SFMLmultiplayerLobbyMenu::launchGame(SFMLshape *)
{
	SFMLloadingGamePopUp	popUp("Loadding...");

	if (!this->_client->CurrentLobby)
		return;
	std::string map = _client->CurrentLobby->getMapName();
	if (map != "UNDEFINED" && map != "")
	{
		_client->askToLaunch();
		_menuManager->goTo("LoadingPopUp");
		popUp.launch();
	}
}
