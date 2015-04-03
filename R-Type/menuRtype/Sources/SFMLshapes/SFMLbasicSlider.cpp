#include "SFMLbasicSlider.h"
#include "databaseSFML.h"

SFMLbasicSlider::SFMLbasicSlider(int minVal, int maxVal, int currVal, const sf::Vector2<int> &pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_minValue = minVal;
	this->_maxValue = maxVal;
	this->_value = currVal;
	this->_area = sf::Rect<int>(pos.x, pos.y, 200, 20);

	this->_background.setTexture(db->getTexture("sliderBar"));
	this->_background.setPosition((float)pos.x, (float)pos.y);

	this->_tick.setTexture(db->getTexture("sliderTick"));
	this->InitSlider();
}


SFMLbasicSlider::~SFMLbasicSlider()
{
}
