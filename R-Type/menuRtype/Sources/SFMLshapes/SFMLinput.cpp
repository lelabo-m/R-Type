#include <iostream>
#include "SFMLinput.h"
#include "databaseSFML.h"

SFMLinput::SFMLinput()
{
	this->InitFunctions();
	this->_textingFocus = false;
	this->_maxMessage = 0;
}

SFMLinput::SFMLinput(int max, const sf::Texture &backgroundTxt, const sf::Rect<int> &area) : SFMLshape(backgroundTxt, area)
{
	this->_maxMessage = max;
	this->InitInputLabel();
	this->InitFunctions();
}

SFMLinput::SFMLinput(int max, const sf::Texture &backgroundTxt, const sf::Vector2<int> &pos) : SFMLshape(backgroundTxt, pos)
{
	this->_maxMessage = max;
	this->InitInputLabel();
	this->InitFunctions();
}

SFMLinput::~SFMLinput()
{
}

void	SFMLinput::InitInputLabel()
{
	this->_message.setPosition((float)_area.left, (float)_area.top + _area.height / 2 - (_message.getCharacterSize() / 2));
	this->_textingFocus = false;
}

void	SFMLinput::InitFunctions()
{
	this->_funcTextingFocus = nullptr;
	this->_funcTextingUnfocus = nullptr;
	this->_funcValidInput = nullptr;
}

void	SFMLinput::addToInput(const std::string &txt)
{
	std::string msg = this->_message.getString();

	if (txt == "Esc" || txt == "Enter")
	{
		this->_textingFocus = false;
		if (txt == "Enter" && this->_funcValidInput)
			this->_funcValidInput(this);
		if (this->_funcTextingUnfocus)
			this->_funcTextingUnfocus(this);
	}
	else if (txt == "Suppr")
	{
		if (msg.size() > 0)
			msg.pop_back();
		this->_message.setString(msg);
	}
	else
	{
		if ((int)msg.size() < this->_maxMessage)
			msg += txt;
		this->_message.setString(msg);
	}
}

void	SFMLinput::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_message);
}

const std::string	SFMLinput::getMessage() const
{
	return	_message.getString().toAnsiString();
}

void	SFMLinput::setMessage(const std::string &msg)
{
	this->_message.setString(msg);
}
