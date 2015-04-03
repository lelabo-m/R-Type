#ifndef _RTYPEGAME_H_
# define _RTYPEGAME_H_

#include	"ISFMLmenu.h"
#include	"windowSFML.h"
#include	"menuManager.h"
#include	"Controller.hpp"
#include	"ClientManager.h"
#include	"GameEventManager.h"
#include	"ObjectRepresentation.h"
#include	"UDPFrame.h"
#include	"ObjectRepresentation.h"

#define	NB_PLAYERS_MAX	4
#define TICK_PER_SEC 50
#define UPDATE_DURATION std::chrono::microseconds((1000000 / TICK_PER_SEC))
#define	TIMEOUTOBJSCENE		20

#define TICK_DURATION (1000 / TICK_PER_SEC)

class RTypeGame : public ISFMLmenu
{
protected:
	int						_freshestTime;
	int						_currentCycle;
	bool					_canDisplay;
	std::string				_menuName;
	sf::Sprite				_background;
	sf::RenderWindow		*_window;
	ClientManager			*_client;
	menuManager				*_menuManager;
	ControllerManager		*_controllerManager;
	GameEventManager		_eventManager;

	std::shared_ptr<Session>		_playerSession;
	std::list<SFMLanimation	*>		_monsterList;
	std::list<SFMLanimation	*>		_monsters;
	std::list<GameMessage *>		_gameMessages;
	std::list<GameMessage*>			_actions;
	std::map<size_t, std::pair<long long, ObjectRepresentation>>	_scene;

private:
	void	initControllers();
	void	pauseGame();
	void	upKey();
	void	leftKey();
	void	rightKey();
	void	downKey();
	void	fireKey();
	void	bigFireKey();

public:
	RTypeGame();
	virtual ~RTypeGame();
	virtual void launch();

private:
	virtual void executeRequests();
	virtual void displayDynamicContents();
	virtual void updateDynamicContents(const sf::Event &);
	virtual void gatherObjectRepresentations();

	void checkScene(long long);
	void gamePlayCycle();
};

#endif