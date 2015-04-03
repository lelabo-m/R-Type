#include <iostream>
#include "SFMLbutton.h"
#include "databaseSFML.h"

SFMLbutton::SFMLbutton() : SFMLshape()
{}

SFMLbutton::SFMLbutton(const std::string &txt, const sf::Texture &sp, const sf::Rect<int> &rc) : SFMLshape(sp, rc)
{
	this->_text.setString(txt);

	this->InitButton();
}

SFMLbutton::SFMLbutton(const sf::Text &txt, const sf::Texture &sp, const sf::Rect<int> &rc) : SFMLshape(sp, rc)
{
	this->_text = txt;

	this->InitButton();
}

SFMLbutton::SFMLbutton(const std::string &txt, const sf::Texture &sp, const sf::Vector2<int> &pos) : SFMLshape(sp, pos)
{
	this->_text.setString(txt);

	this->InitButton();
}

SFMLbutton::SFMLbutton(const sf::Text &txt, const sf::Texture &sp, const sf::Vector2<int> &pos) : SFMLshape(sp, pos)
{
	this->_text = txt;

	this->InitButton();
}

SFMLbutton::~SFMLbutton()
{
}

const SFMLbutton &SFMLbutton::operator=(const SFMLbutton &oth)
{
	this->_text = oth._text;
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

const SFMLbutton &SFMLbutton::operator=(const SFMLbutton &&oth)
{
	this->_text = oth._text;
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

void SFMLbutton::InitButton()
{
	this->_text.setPosition((float)_area.left + (_area.width - _text.getLocalBounds().width) / 2,
							(float)_area.top + _area.height / 2 - (float)(_text.getCharacterSize() * 0.75));
}

void SFMLbutton::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_text);
}
