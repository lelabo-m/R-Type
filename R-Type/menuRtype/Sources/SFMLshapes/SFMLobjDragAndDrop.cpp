#include "SFMLshape.h"
#include "SFMLobjDragAndDrop.h"

SFMLobjDragAndDrop::SFMLobjDragAndDrop()
{
	this->_click = false;
	this->initFunctions();
}

SFMLobjDragAndDrop::SFMLobjDragAndDrop(const sf::Texture &texture, const sf::Vector2<int> &pos) : SFMLshape(texture, pos)
{
	this->_click = false;
	this->initFunctions();
}

SFMLobjDragAndDrop::~SFMLobjDragAndDrop()
{
}

const SFMLobjDragAndDrop &SFMLobjDragAndDrop::operator=(const SFMLobjDragAndDrop &oth)
{
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;
	this->_click = oth._click;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

const SFMLobjDragAndDrop &SFMLobjDragAndDrop::operator=(const SFMLobjDragAndDrop &&oth)
{
	this->_background = oth._background;
	this->_area = oth._area;
	this->_focus = oth._focus;
	this->_click = oth._click;

	this->_funcFocus = oth._funcFocus;
	this->_funcUnfocus = oth._funcUnfocus;
	this->_funcClick = oth._funcClick;
	this->_funcRelease = oth._funcRelease;

	return *this;
}

void	SFMLobjDragAndDrop::changingPosition(int xMousePos, int yMousePos)
{
	this->_background.setPosition((float)(xMousePos - _background.getLocalBounds().width / 2), (float)(yMousePos - _background.getLocalBounds().height / 2));
	this->_area.left	= (int)(xMousePos - _background.getLocalBounds().width / 2);
	this->_area.top		= (int)(yMousePos - _background.getLocalBounds().height / 2);

	if (this->_funcPositionChanged)
		this->_funcPositionChanged(this);
}

void	SFMLobjDragAndDrop::initFunctions()
{
	this->_funcPositionChanged = nullptr;
}
