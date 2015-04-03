#include <chrono>
#include "RTypeGame.h"
#include "SFMLpausePopUp.h"
#include "gameOptions.h"
#include "GameMessageDispacher.hpp"
#include "PlayerAction.h"
#include "Level.hh"
#include "SFMLmonsterDnD.h"
#include "BackgroundAnimation.h"
#include "SFMLendingGamePopUp.h"
#include "databaseSFML.h"
#include "GameAction.hh"
#include "Playlist.hh"
#include "MapSelected.h"

RTypeGame::RTypeGame() : _background(databaseSFML::getDatabase()->getTexture("backgroundGame2"))
{
	this->_menuName = "RTypeGame";
	this->_canDisplay = true;
	this->_controllerManager = ControllerManager::GetInstance();
	this->_menuManager = menuManager::getInst();
	this->_window = windowSFML::getWindow();
	this->_client = ClientManager::getInstance();
	this->_currentCycle = 0;
	this->initControllers();

	_playerSession = this->_client->getNetClient().getOwnSession();
	_playerSession.get()->clearUDPReceivedFrame();
}

RTypeGame::~RTypeGame()
{
	_client->clearGraphicalElements();
	_playerSession.get()->clearUDPReceivedFrame();

}

void	RTypeGame::initControllers()
{
	gameOptions *opts = gameOptions::getGameOptions();

	this->_eventManager.add(opts->getKeyUp(), std::bind(&RTypeGame::upKey, this));
	this->_eventManager.add(opts->getKeyDown(), std::bind(&RTypeGame::downKey, this));
	this->_eventManager.add(opts->getKeyLeft(), std::bind(&RTypeGame::leftKey, this));
	this->_eventManager.add(opts->getKeyRight(), std::bind(&RTypeGame::rightKey, this));
	this->_eventManager.add(opts->getKeyFire(), std::bind(&RTypeGame::fireKey, this));
	this->_eventManager.add("Esc", std::bind(&RTypeGame::pauseGame, this));
	this->_eventManager.add("b", std::bind(&RTypeGame::bigFireKey, this));
}

void	RTypeGame::launch()
{
	sf::Event				ev;
	BackgroundAnimation		animation(0, 2800, 0, 3, SLIDER);
	ControllerList cl = ControllerManager::GetInstance()->ControllersByType(ControllerType::KEYBOARD);

	// UDP PUNCHING
	leftKey();

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	while (this->_menuName == _menuManager->getCurrentMenu())
	{
		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
		this->executeRequests();

		animation.next();
		_background.setTextureRect(sf::Rect<int>(animation.getValue(), 0, 1280, 875));

		while (this->_window->pollEvent(ev))
		{
			this->_controllerManager->Update(ev);
			this->_eventManager.execute(ev);
			this->updateDynamicContents(ev);
		}
		this->_eventManager.executeKeyboard(dynamic_cast<KeyBoard *>(cl.front()));
		gamePlayCycle();


		if (_canDisplay)
		{
			this->_window->clear();
			this->displayDynamicContents();
			this->_window->display();
		}

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		{
			auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (time / 2000000 > NETWORK_TIMER)
			{
				try
				{
					this->_client->handleNetwork();
				}
				catch (std::runtime_error &)
				{
					start = std::chrono::high_resolution_clock::now();
				}
			}
			this->checkScene(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
			auto timing = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			// (CYCLE_TIME_MENU - timing) -> valeur toujours negative et des fois au retour  a un menu precedent
			//  les valeurs sont plus grande que d'habitude (x10 environ), ce qui cause le freeze sur l'appel systeme nanosleep()
			//  ------> nanosleep({4294967295, 0});
			if (timing < UPDATE_DURATION)
				std::this_thread::sleep_for(UPDATE_DURATION - timing);
		}

		
	}
}

void RTypeGame::checkScene(long long timer)
{

	std::map<size_t, std::pair<long long, ObjectRepresentation>>::iterator it;

	for (it = _scene.begin(); it != _scene.end();)
	{
		it->second.first -= timer;
		if (it->second.first <= 0)
			it = _scene.erase(it);
		else
			it++;
	}
}

void RTypeGame::gatherObjectRepresentations()
{
	_canDisplay = false;
	for (unsigned int i = _playerSession.get()->countUDPReceivedFrame(); i > 0; i--)
	{
		UDPFrame *frame = _playerSession.get()->getUDPReceivedFrame();
		frame->getHeader().setFromData(frame->getData());
		if (_currentCycle <= frame->getHeader().getTime())
		{
			_currentCycle = frame->getHeader().getTime();
			_canDisplay = true;
			GameMessageDispacher::extractFrame(frame, _gameMessages, 0);

		}
		frame->release();
	}
}

void RTypeGame::gamePlayCycle()
{
	for (auto it = _gameMessages.begin(); it != _gameMessages.end(); it++)
	{
		ObjectRepresentation *objRep = (*it)->unserializeContent<ObjectRepresentation>();

		if (_scene.find(objRep->getObjectID()) == _scene.end())
		{
			for (const auto &elem : _client->getGraphicalElements())
				if (static_cast<int>(elem.ClassId()) == objRep->getClassID())
				{
					if (elem.Name() == "PlayerBullet")
						Playlist::getPlaylist()->playSound("blast");
					break;
				}
		}

		_scene[objRep->getObjectID()].first = TIMEOUTOBJSCENE;
		_scene[objRep->getObjectID()].second = *objRep;

		delete objRep;
		delete (*it);
	}
	_gameMessages.clear();

	while (_actions.size() > 0)
	{
		UDPFrame	*frame = (&(_client->getNetClient()))->getFramePool().createUDPFrame();


		frame->getHeader().setMagicNumber(42);
		frame->getHeader().setSessionID(_playerSession.get()->getSessionID());
		frame->getHeader().setTime(_currentCycle);
		frame->getHeader().setDataFromHeader(frame->getData());

		GameMessageDispacher::fillFrame(frame, _actions);

		_playerSession.get()->pushUDPToSendFrame(frame);
	}
}

void	RTypeGame::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
	if (_client->CurrentGame->getGameStatus() == Game::GAME_FINISHED)
	{
		SFMLendingGamePopUp		popUp;

		_menuManager->goTo("EndingGamePopUp");
		popUp.launch();
	}
	this->gatherObjectRepresentations();
}

void	RTypeGame::displayDynamicContents()
{
	std::list<GraphicalElem> listGraphical = _client->getGraphicalElements();

	this->_window->draw(_background);
	for (auto &elemScene : _scene)
	{		
		for (auto &elem : listGraphical)
		{
			if ((elemScene.second.second).getClassID() == static_cast<int>(elem.ClassId()))
			{
				SFMLanimation *	anim = _client->AnimManager().GetAnimation(	elemScene.second.second.getClassID(),
																			elemScene.second.second.getAnimation());
				if (anim)
					anim->PlayLoop(elemScene.second.second.getPosX(), elemScene.second.second.getPosY(), *_window);
			}
		}
	}
}

void	RTypeGame::updateDynamicContents(const sf::Event &)
{

}

void	RTypeGame::pauseGame()
{
	SFMLpausePopUp	popUp;

	_menuManager->goTo("PausePopUp");
	popUp.launch();
}

void	RTypeGame::upKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::UP);
	new_action->serialize();

	_actions.push_back(new_action);
}

void	RTypeGame::leftKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::LEFT);
	new_action->serialize();

	_actions.push_back(new_action);
}

void	RTypeGame::rightKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::RIGHT);
	new_action->serialize();

	_actions.push_back(new_action);
}

void	RTypeGame::downKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::DOWN);
	new_action->serialize();

	_actions.push_back(new_action);
}

void	RTypeGame::fireKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::FIRE);
	new_action->serialize();

	_actions.push_back(new_action);
}

void	RTypeGame::bigFireKey()
{
	GameMessage *new_action = new GameMessage();
	new_action->serializeContent<PlayerAction>(Gpa::Action::BONUS);
	new_action->serialize();

	_actions.push_back(new_action);
}