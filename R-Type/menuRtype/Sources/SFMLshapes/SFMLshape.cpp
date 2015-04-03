#include <iostream>
#include "SFMLshape.h"

SFMLshape::SFMLshape()
{
	this->_focus = false;
	this->InitFunctions();
}

SFMLshape::SFMLshape(const sf::Texture &background, const sf::Rect<int> &area)
{
	this->_background.setTexture(background);
	this->_area = area;
	this->InitForm();
	this->InitFunctions();
}

SFMLshape::SFMLshape(const sf::Texture &background, const sf::Vector2<int> &beginArea)
{
	this->_background.setTexture(background);
	this->_area = sf::Rect<int>(beginArea.x, beginArea.y,
								(int)this->_background.getLocalBounds().width,
								(int)this->_background.getLocalBounds().height);
	this->InitForm();
	this->InitFunctions();
}

SFMLshape::~SFMLshape()
{
}

void SFMLshape::InitForm()
{
	this->_focus = false;
	this->_background.setPosition((float)_area.left, (float)_area.top);
}

void SFMLshape::InitFunctions()
{
	this->_funcFocus = nullptr;
	this->_funcUnfocus = nullptr;
	this->_funcClick = nullptr;
	this->_funcRelease = nullptr;
}

const SFMLshape &SFMLshape::operator=(const SFMLshape &oth)
{
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;

	this->_funcFocus = oth._funcFocus;
	this->_funcClick = oth._funcClick;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

const SFMLshape &SFMLshape::operator=(const SFMLshape &&oth)
{
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = false;

	this->_funcFocus = oth._funcFocus;
	this->_funcClick = oth._funcClick;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

bool	SFMLshape::isFocusChanged(const sf::Vector2<int> &mousePos)
{
	bool	retVal = (this->_area.contains(mousePos) != this->_focus);
	this->_focus = this->_area.contains(mousePos);
	return retVal;
}

bool	SFMLshape::isFocusChanged(int posX, int posY)
{
	bool	retVal = (this->_area.contains(posX, posY) != this->_focus);
	this->_focus = this->_area.contains(posX, posY);
	return retVal;
}

bool	SFMLshape::isFocus() const
{
	return this->_focus;
}

void SFMLshape::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
}

const sf::Rect<int>	&SFMLshape::getPosition() const
{
	return this->_area;
}

void SFMLshape::setPosition(const sf::Rect<int> &area)
{
	this->_area = area;
	this->_background.setPosition((float)area.left, (float)area.top);
	this->_background.setTextureRect(sf::Rect<int>(0, 0, area.width, area.height));
}
