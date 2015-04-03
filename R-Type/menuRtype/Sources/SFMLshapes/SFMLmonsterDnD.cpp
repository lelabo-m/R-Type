#include <iostream>
#include "SFMLmonsterDnD.h"

SFMLmonsterDnD::SFMLmonsterDnD(const sf::Texture &texture, const std::string &name, const sf::Vector2<int> &pos, const Time &timer) : SFMLobjDragAndDrop(texture, pos)
{
	this->_textingFocus = false;
	this->_toDelete		= false;
	this->_isVisible	= true;
	this->_name			= name;
	this->_spawnTimer	= timer;
	this->_spawnPos		= pos;
}

SFMLmonsterDnD::~SFMLmonsterDnD()
{}

void	SFMLmonsterDnD::changingPosition(int, int mouseY)
{
	mouseY = std::max(mouseY, 98);
	mouseY = std::min(mouseY, 798);

	this->_background.setPosition((float)this->_area.left, (float)(mouseY - _background.getLocalBounds().height / 2));
	this->_area.top = (int)(mouseY - _background.getLocalBounds().height / 2);
	this->_spawnPos.y = (int)(mouseY - _background.getLocalBounds().height / 2);

	if (this->_funcPositionChanged)
		this->_funcPositionChanged(this);
}

bool	SFMLmonsterDnD::hasToDelete() const
{
	return _toDelete;
}

bool	SFMLmonsterDnD::isVisible() const
{
	return _isVisible;
}

void	SFMLmonsterDnD::addToInput(const std::string &txt)
{
	if (txt == "Suppr")
	{
		_toDelete = true;
	}
}

void	SFMLmonsterDnD::checkVisibility(const Time &time)
{
	long int		timeGiven = (time.Minutes * 60 * 1000) + (time.Seconds * 1000) + time.Miliseconds;
	long int		timeMonster = (_spawnTimer.Minutes * 60 * 1000) + (_spawnTimer.Seconds * 1000) + (_spawnTimer.Miliseconds);

	_isVisible = (timeGiven - 10000 <= timeMonster && timeGiven >= timeMonster) ? true : false;
}

void	SFMLmonsterDnD::updateXPosition(const Time &time)
{
	long int		timeGiven = (time.Minutes * 60 * 1000) + (time.Seconds * 1000) + time.Miliseconds;
	long int		timeMonster = (_spawnTimer.Minutes * 60 * 1000) + (_spawnTimer.Seconds * 1000) + (_spawnTimer.Miliseconds);
	long int		xPos = 1200 - (timeGiven - timeMonster) / 10;

	this->_area.left = (int)xPos - int(_background.getLocalBounds().width / 2);
	this->_background.setPosition((float)_area.left, (float)_area.top);
}

const Time &SFMLmonsterDnD::getTimer() const
{
	return this->_spawnTimer;
}

const sf::Vector2<int>	&SFMLmonsterDnD::getSpawnPos() const
{
	return this->_spawnPos;
}

const std::string	&SFMLmonsterDnD::getName() const
{
	return this->_name;
}
