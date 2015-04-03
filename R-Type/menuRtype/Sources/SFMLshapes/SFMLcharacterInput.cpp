#include "SFMLcharacterInput.h"
#include "SFMLbasicText.h"
#include "gameOptions.h"

SFMLcharacterInput::SFMLcharacterInput()
{

}

SFMLcharacterInput::SFMLcharacterInput(const sf::Vector2<int> &pos, keysOpt keyAssigned, const std::string &val)
{
	this->_key = keyAssigned;
	this->_maxMessage = 1;
	this->_message = SFMLbasicText(val, pos);
	this->_area = sf::Rect<int>(pos.x, pos.y, 30, 30);
	InitInputLabel();
}


SFMLcharacterInput::~SFMLcharacterInput()
{
}

#include <iostream>

void	SFMLcharacterInput::addToInput(const std::string &txt)
{
	gameOptions		*opts = gameOptions::getGameOptions();
	std::string msg = this->_message.getString();

	if (txt == "Esc" || txt == "Enter")
	{
		this->_textingFocus = false;
		if (this->_funcTextingUnfocus)
			this->_funcTextingUnfocus(this);
	}
	else if (txt != "Suppr")
	{
		if (opts->hasKey(txt) == false)
		{
			opts->setKey(this->_key, txt);
			this->_message.setString(txt);
		}
	}

}
