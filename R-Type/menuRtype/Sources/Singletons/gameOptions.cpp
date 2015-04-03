#include <iostream>
#include "gameOptions.h"

gameOptions *gameOptions::inst = NULL;

gameOptions::gameOptions()
{
	_volumeBackground = 100;
	_volumeSound = 100;
	_keys[UP]		= "z";
	_keys[LEFT]		= "q";
	_keys[DOWN]		= "s";
	_keys[RIGHT]	= "d";
	_keys[FIRE]		= "f";

	_nickName	= "NonePlayer";
}

gameOptions::~gameOptions()
{
}

gameOptions *gameOptions::getGameOptions()
{
	if (inst == NULL)
		inst = new gameOptions;
	return inst;
}

void gameOptions::deleteGameOptions()
{
	if (inst)
		delete inst;
	inst = NULL;
}

bool	gameOptions::hasKey(const std::string &key) const
{
	for (const auto &vals : this->_keys)
	{
		if (vals.second == key)
			return true;
	}
	return false;
}

void	gameOptions::setKey(keysOpt val, const std::string &key)
{
	_keys[val] = key;
}

const std::string	&gameOptions::getKeyUp() const
{
	return	_keys.at(UP);
}

const std::string	&gameOptions::getKeyDown() const
{
	return	_keys.at(DOWN);
}

const std::string	&gameOptions::getKeyLeft() const
{
	return	_keys.at(LEFT);
}

const std::string	&gameOptions::getKeyRight() const
{
	return	_keys.at(RIGHT);
}

const std::string	&gameOptions::getKeyFire() const
{
	return	_keys.at(FIRE);
}

const std::string	&gameOptions::getNickName() const
{
	return	_nickName;
}

int					gameOptions::getVolumeBackground() const
{
	return	_volumeBackground;
}
int					gameOptions::getVolumeSound() const
{
	return	_volumeSound;
}

void	gameOptions::setKeyUp(const std::string &key)
{
	_keys[UP] = key;
}

void	gameOptions::setKeyDown(const std::string &key)
{
	_keys[DOWN] = key;
}

void	gameOptions::setKeyLeft(const std::string &key)
{
	_keys[LEFT] = key;
}

void	gameOptions::setKeyFire(const std::string &key)
{
	_keys[FIRE] = key;
}

void	gameOptions::setKeyRight(const std::string &key)
{
	_keys[RIGHT] = key;
}

void	gameOptions::setNickName(const std::string &nick)
{
	_nickName = nick;
}

void	gameOptions::setVolumeBackground(int vol)
{
	_volumeBackground = vol;
}

void	gameOptions::setVolumeSound(int vol)
{
	_volumeSound = vol;
}
