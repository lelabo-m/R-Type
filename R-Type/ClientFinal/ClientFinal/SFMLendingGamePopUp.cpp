#include "SFMLendingGamePopUp.h"


SFMLendingGamePopUp::SFMLendingGamePopUp()
{
	this->_menuName = "EndingGamePopUp";
	this->_quit = new SFMLbuttonLauncher(sf::Vector2<int>(549, 350), "Return to menu");
	this->_quit->_funcRelease = std::bind(&SFMLendingGamePopUp::Quit, this, _quit);

	_title = SFMLbasicText("Game Over", sf::Vector2<int>());
	_scoreText = SFMLbasicText("Final score : " + std::to_string(_client->CurrentGame->getScore()) + " points", sf::Vector2<int>(), 20);

	_title.setPosition(sf::Vector2<float>(640 - _title.getLocalBounds().width / 2, 75));
	_scoreText.setPosition(sf::Vector2<float>(640 - _scoreText.getLocalBounds().width / 2, 120));

	this->_texts.push_back(_title);
	this->_texts.push_back(_scoreText);
	this->_forms.push_back(_quit);
	this->_eventManager.add(_quit);
}

SFMLendingGamePopUp::~SFMLendingGamePopUp()
{
	delete	this->_quit;
}

void	SFMLendingGamePopUp::Quit(SFMLshape *)
{
	_menuManager->goTo("Main");
}