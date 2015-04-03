#include <iostream>
#include "SFMLmainMenu.h"
#include "Playlist.hh"
#include "databaseSFML.h"
#include "SFMLbuttonevents.h"
#include "MapSelected.h"
#include "SFMLoptionMenu.h"
#include "SFMLplayMenu.h"
#include "SFMLplaymapSelectorMenu.h"
#include "SFMLlevelscoreMenu.h"

SFMLmainMenu::SFMLmainMenu()
{
	this->_menuName = "Main";
	this->initBaseMainMenu();
}

SFMLmainMenu::~SFMLmainMenu()
{
	delete 	_play;
	delete	_option;
	delete 	_scores;
	delete 	_exit;
}

void	SFMLmainMenu::initBaseMainMenu()
{
	_play		= new SFMLlittleButton("Play", sf::Vector2<int>(60, 762));
	_option		= new SFMLlittleButton("Options", sf::Vector2<int>(252, 762));
	_scores		= new SFMLlittleButton("Scores", sf::Vector2<int>(440, 762));
	_exit		= new SFMLlittleButton("Exit", sf::Vector2<int>(630, 762));

	_play->_funcRelease		= std::bind(&SFMLmainMenu::Play, this, _play);
	_option->_funcRelease	= std::bind(&SFMLmainMenu::Options, this, _option);
	_scores->_funcRelease	= std::bind(&SFMLmainMenu::Scores, this, _scores);
	_exit->_funcRelease		= std::bind(&SFMLmainMenu::Cancel, this, _exit);

	this->_forms.push_back(_play);
	this->_eventManager.add(_play);

	this->_forms.push_back(_option);
	this->_eventManager.add(_option);

	this->_forms.push_back(_scores);
	this->_eventManager.add(_scores);

	this->_forms.push_back(_exit);
	this->_eventManager.add(_exit);
}

void	SFMLmainMenu::Cancel(SFMLshape *)
{
	_menuManager->goTo("");
}

void	SFMLmainMenu::Play(SFMLshape *)
{
	SFMLplayMenu	playMenu;

	_menuManager->goTo("Play");
	playMenu.launch();
}

void	SFMLmainMenu::Scores(SFMLshape *)
{
	SFMLplaymapSelectorMenu selectorMenu;
	MapSelected	*mapSelected = MapSelected::getInst();

	_menuManager->goTo("Mapselector");
	selectorMenu.launch();

	std::string		level(mapSelected->getMap());
	if (level != "")
	{
		std::cout << level << std::endl;
		level = FileSystem::FileName(level);

		SFMLlevelscoreMenu menu(level);

		_menuManager->goTo("LevelScore");
		menu.launch();
	}
}

void	SFMLmainMenu::Options(SFMLshape *)
{
	SFMLoptionMenu optionMenu;

	_menuManager->goTo("Options");
	optionMenu.launch();
}
