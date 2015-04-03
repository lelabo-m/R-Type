#include <iostream>
#include "SFMLslider.h"

SFMLslider::SFMLslider()
{
	this->_minValue = 0;
	this->_maxValue = 0;
	this->_value = 0;

	this->_click = false;
	this->InitFunctions();
}

SFMLslider::SFMLslider(int minVal, int maxVal, const sf::Texture &barTxt, const sf::Texture &tickTxt, const sf::Vector2<int> &pos) : SFMLshape(barTxt, pos)
{
	this->_minValue = minVal;
	this->_maxValue = maxVal;
	this->_value = minVal;

	this->_tick.setTexture(tickTxt);
	this->InitSlider();
	this->InitFunctions();
}

SFMLslider::SFMLslider(int minVal, int maxVal, int initVal, const sf::Texture &barTxt, const sf::Texture &tickTxt, const sf::Vector2<int> &pos) : SFMLshape(barTxt, pos)
{
	this->_minValue = minVal;
	this->_maxValue = maxVal;
	this->_value = initVal;

	this->_tick.setTexture(tickTxt);
	this->InitFunctions();
	this->InitSlider();
}

const SFMLslider &SFMLslider::operator=(const SFMLslider &oth)
{
	this->_tick = oth._tick;
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;
	this->_click = oth._click;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;
	this->_funcValueChanged = oth._funcValueChanged;

	return *this;
}

const SFMLslider &SFMLslider::operator=(const SFMLslider &&oth)
{
	this->_tick = oth._tick;
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;
	this->_click = oth._click;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;
	this->_funcValueChanged = oth._funcValueChanged;

	return *this;
}

void SFMLslider::InitSlider()
{
	float xpos;

	xpos = (float)(this->_area.left + ((float)(this->_value - this->_minValue) / (this->_maxValue - this->_minValue) * this->_area.width));
	this->_tick.setPosition(xpos, (float)_area.top);
	this->_click = false;
}

void	SFMLslider::InitFunctions()
{
	this->_funcValueChanged = nullptr;
}

SFMLslider::~SFMLslider()
{

}

void	SFMLslider::changingValue(int xMousePos)
{
	if (xMousePos < this->_area.left)
		xMousePos = this->_area.left;
	else if (xMousePos > this->_area.left + this->_area.width)
		xMousePos = this->_area.left + this->_area.width;

	this->_tick.setPosition((float)xMousePos - (_tick.getLocalBounds().width / 2), (float)_area.top);
	this->_value = (int)((float)(xMousePos - this->_area.left) / (this->_area.width) * (this->_maxValue - this->_minValue)) + this->_minValue;
	if (this->_funcValueChanged)
		this->_funcValueChanged(this);
}

void	SFMLslider::updateSlider(int value)
{
	this->_value = value;
	this->_tick.setPosition(this->_area.left + (((float)(this->_value - this->_minValue) / (this->_maxValue - this->_minValue) * _area.width) - (_tick.getLocalBounds().width / 2)), (float)_area.top);
}

void	SFMLslider::Draw(sf::RenderWindow &window)
{
	window.draw(this->_background);
	window.draw(this->_tick);
}

int		SFMLslider::getValue() const
{
	return	_value;
}

void	SFMLslider::setMaxValue(int value)
{
	this->_maxValue = value;
	this->_value = std::min(this->_value, this->_maxValue);
}

void	SFMLslider::setMinValue(int value)
{
	this->_minValue = value;
	this->_value = std::max(this->_value, this->_minValue);
}
