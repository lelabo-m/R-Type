#include "SFMLbasicInput.h"
#include "databaseSFML.h"
#include "SFMLinputevents.h"
#include "SFMLbasicText.h"

SFMLbasicInput::SFMLbasicInput(int valMax, const sf::Vector2<int> &pos, const std::string &txt)
{

	this->_maxMessage = valMax;
	this->_message = SFMLbasicText(txt, pos);
	this->_area = sf::Rect<int>(pos.x, pos.y, valMax * 30, 30);

	this->InitInputLabel();
}

SFMLbasicInput::~SFMLbasicInput()
{
}
