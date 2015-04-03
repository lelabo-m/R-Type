#pragma once

#include <list>
#include "ISFMLmenu.h"
#include "ClientManager.h"
#include "windowSFML.h"
#include "EditorEventManager.h"
#include "SFMLshape.h"
#include "SFMLmonsterDnD.h"
#include "SFMLmonsterButton.h"
#include "SFMLbasicText.h"
#include "menuManager.h"
#include "Timer.h"

class SFMLbaseEditor :
	public ISFMLmenu
{
protected:
	sf::RenderWindow				*_window;
	ClientManager					*_client;
	EditorEventManager				_eventManager;
	menuManager						*_menuManager;
	std::list<sf::Sprite>			_images;
	std::list<sf::Text>				_texts;
	std::list<SFMLshape *>			_forms;
	std::list<SFMLanimation	*>		_monsterList;
	std::list<SFMLmonsterDnD *>		_monsters;
	SFMLbasicText					_time;
	Timer							_timer;
	int								_secsTimeline;
	std::string						_menuName;

public:
	SFMLbaseEditor();
	virtual ~SFMLbaseEditor();

public:
	void	initBaseMenu();
	void	launch();
	virtual void	updateContent();
	virtual void	executeRequests();
	virtual void	updateDynamicContents(const sf::Event &);
	virtual void	displayDynamicContents();

private:

};