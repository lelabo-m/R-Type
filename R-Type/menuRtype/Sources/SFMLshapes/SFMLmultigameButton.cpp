#include <iostream>
#include "SFMLmultigameButton.h"
#include "SFMLmultiplayerLobbyMenu.h"
#include "databaseSFML.h"
#include "TickTimer.h"
#include "gameOptions.h"

SFMLmultigameButton::SFMLmultigameButton(const sf::Vector2<int> &pos, const std::string &gameName, const std::string &desc, const std::string &nbPlayer)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("multigameButton"));
	_text = SFMLbasicText(gameName, pos, 20);
	_desc = SFMLbasicText(desc, pos, 12);
	_nbPlayers = SFMLbasicText(nbPlayer, pos, 12);
	this->setPosition(sf::Rect<int>(pos.x, pos.y, 300, 50));

	this->_funcFocus	= std::bind(&SFMLmultigameButton::hover, this, this);
	this->_funcUnfocus	= std::bind(&SFMLmultigameButton::unhover, this, this);
	this->_funcRelease	= std::bind(&SFMLmultigameButton::click, this, this);
}

SFMLmultigameButton::~SFMLmultigameButton()
{
}

void	SFMLmultigameButton::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_text);
	window.draw(this->_desc);
	window.draw(this->_nbPlayers);
}

void	SFMLmultigameButton::setPosition(const sf::Rect<int> &area)
{
	this->_area = area;
	this->_background.setPosition(static_cast<float>(area.left), static_cast<float>(area.top));
	this->_text.setPosition(static_cast<float>(area.left + (area.width / 2) - (_text.getLocalBounds().width / 2)), static_cast<float>(area.top + 5));
	this->_desc.setPosition(static_cast<float>(area.left + 5), static_cast<float>(area.top + 30));
	this->_nbPlayers.setPosition(static_cast<float>(area.left + (area.width / 2) - (_text.getLocalBounds().width / 2) + 180), static_cast<float>(area.top + 30));

}

void	SFMLmultigameButton::hover(SFMLshape *)
{
	databaseSFML *db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("multigameButtonHover"));
}

void	SFMLmultigameButton::unhover(SFMLshape *)
{
	databaseSFML *db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("multigameButton"));
}

void	SFMLmultigameButton::outTimeLobbyAnswer()
{
	throw std::runtime_error("Request for joining lobby is not responding");
}

void	SFMLmultigameButton::click(SFMLshape *)
{

	ClientManager				*client = ClientManager::getInstance();
	gameOptions					*opts = gameOptions::getGameOptions();
	menuManager					*menuManager = menuManager::getInst();
	TickTimer					_timer(5, std::bind(&SFMLmultigameButton::outTimeLobbyAnswer, this));

	if (client->CurrentLobby)
		return ;

	try
	{
		client->askJoinGame(_text.getString().toAnsiString(), opts->getNickName());
		client->CurrentLobby = new Lobby(_text.getString().toAnsiString(), "UNDEFINED", opts->getNickName());

		while (client->CurrentLobby && client->CurrentLobby->isConfirmed() == false)
		{
			_timer.update();
			client->handleNetwork();
			std::cout << "Checking" << std::endl;
		}

		std::cout << client->CurrentLobby << std::endl;
		if (client->CurrentLobby)
		{
			SFMLmultiplayerLobbyMenu	lobby(GUEST);

			menuManager->goTo("MultiplayerLobby");
			lobby.launch();


		}
	}
	catch (std::runtime_error &)
	{}

	std::cout << "Try to deleting" << std::endl;
	if (client->CurrentLobby)
	{
		std::cout << "delete" << std::endl;
		delete client->CurrentLobby;
		client->CurrentLobby = nullptr;
	}
}
