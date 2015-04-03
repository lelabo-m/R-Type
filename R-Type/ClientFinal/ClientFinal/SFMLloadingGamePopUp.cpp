#include "SFMLloadingGamePopUp.h"
#include "RTypeGame.h"
#include "SFMLbasicText.h"

SFMLloadingGamePopUp::SFMLloadingGamePopUp(const std::string &msg)
{
	this->_menuName = "LoadingPopUp";
	this->_text = SFMLbasicText(msg, sf::Vector2<int>(0, 0), 20);
	this->_text.setPosition(640 - this->_text.getLocalBounds().width / 2, 310);

	this->_mapStatus[Game::WAITING] = "Waiting...";
	this->_mapStatus[Game::LOADING] = "Loading...";
	this->_mapStatus[Game::LOADING_FINISH] = "Loading has been finished.";
	this->_mapStatus[Game::SYNCHRONIZATION] = "Synchronization...";
	this->_mapStatus[Game::SYNCHRONIZATION_FINISHED] = "Syncronization has been finished.";
	this->_mapStatus[Game::GAME_STARTED] = "Game is starting.";
	this->_mapStatus[Game::GAME_INTERUPTION] = "Game has been interupted.";
}

SFMLloadingGamePopUp::~SFMLloadingGamePopUp()
{
}

void	SFMLloadingGamePopUp::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}

	if (this->_client->CurrentGame)
	{
		this->_text.setString(this->_mapStatus[this->_client->CurrentGame->getGameStatus()]);
		this->_text.setPosition(640 - this->_text.getLocalBounds().width / 2, 310);
		if (this->_client->CurrentGame->getGameStatus() == Game::GAME_STARTED)
		{
			RTypeGame	game;

			_menuManager->goTo("RTypeGame");

			std::this_thread::sleep_for(std::chrono::seconds(2));
			game.launch();
		}
	}
}

void	SFMLloadingGamePopUp::displayDynaicContents()
{
	this->_window->draw(this->_text);
}