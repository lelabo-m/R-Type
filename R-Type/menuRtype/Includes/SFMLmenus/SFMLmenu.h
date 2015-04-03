#pragma once

#include	<list>
#include 	<string>
#include	<SFML/Graphics.hpp>
#include	"ClientManager.h"
#include	"EventManager.h"
#include	"ISFMLmenu.h"
#include	"windowSFML.h"
#include	"menuManager.h"
#include	"Controller.hpp"

class SFMLmenu : public ISFMLmenu
{
protected:
	sf::RenderWindow		*_window;
	ClientManager			*_client;
	menuManager				*_menuManager;
	ControllerManager		*_controllerManager;
	EventManager			_eventManager;
	std::list<sf::Sprite>	_images;
	std::list<sf::Text>		_texts;
	std::list<SFMLshape *>	_forms;
	std::string 			_menuName;

public:
	SFMLmenu();
	virtual ~SFMLmenu();

private:
	void initBaseMenu();
	virtual void executeRequests();
	virtual void updateDynamicContents(const sf::Event &ev);
	virtual void displayDynaicContents();

public:
	virtual void launch();
};

