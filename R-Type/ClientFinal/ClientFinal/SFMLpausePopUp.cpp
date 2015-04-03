#include "SFMLpausePopUp.h"
#include "SFMLbasicText.h"

SFMLpausePopUp::SFMLpausePopUp()
{
	this->_menuName = "PausePopUp";
	this->initPopUp();
}


SFMLpausePopUp::~SFMLpausePopUp()
{
	delete	_unpause;
	delete	_quit;
}

void	SFMLpausePopUp::initPopUp()
{
	SFMLbasicText	title("The game has been paused", sf::Vector2<int>());

	title.setPosition(sf::Vector2<float>(640 - title.getLocalBounds().width / 2, 75));
	this->_unpause	= new SFMLbuttonLauncher(sf::Vector2<int>(549, 270), "Unpause");
	this->_quit		= new SFMLbuttonLauncher(sf::Vector2<int>(549, 400), "Quit");

	_unpause->_funcRelease	= std::bind(&SFMLpausePopUp::unpause, this, _unpause);
	_quit->_funcRelease		= std::bind(&SFMLpausePopUp::quit, this, _quit);

	_eventManager.add(_unpause);
	_eventManager.add(_quit);

	_forms.push_back(_unpause);
	_forms.push_back(_quit);

	_texts.push_back(title);
}

void	SFMLpausePopUp::unpause(SFMLshape *)
{
	_menuManager->goTo("RTypeGame");
}

void	SFMLpausePopUp::quit(SFMLshape *)
{
	//_client->askLeaveLobby();
	_menuManager->goTo("Main");
}