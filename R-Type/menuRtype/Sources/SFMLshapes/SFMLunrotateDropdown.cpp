#include <iostream>
#include "SFMLunrotateDropdown.h"
#include "SFMLchatText.h"
#include "databaseSFML.h"


SFMLunrotateDropdown::SFMLunrotateDropdown(	const sf::Texture &upButton, const sf::Vector2<int> &posUp,
											const sf::Texture &downButton, const sf::Vector2<int> &posDown,
											const sf::Vector2<int> &posObjs, int nbVisible, dropdownType type)
: SFMLdropdown(upButton, posUp, downButton, posDown, posObjs, nbVisible)
{
	this->initDropdown();
	this->_type = type;
}

SFMLunrotateDropdown::~SFMLunrotateDropdown()
{
}

void	SFMLunrotateDropdown::initDropdown()
{
	this->_currentStage = 0;
	this->_up->_funcRelease		= std::bind(&SFMLunrotateDropdown::upButton, this, this->_up);
	this->_down->_funcRelease	= std::bind(&SFMLunrotateDropdown::downButton, this, this->_down);
}

void	SFMLunrotateDropdown::upButton(SFMLshape *)
{
	sf::Rect<int>	area;

	if (this->_objs.size() > 1 && static_cast<size_t>(_currentStage) < this->_objs.size() - 1)
	{
		for (auto &monster : this->_objs)
		{
			area = monster->getPosition();
			area.top -= area.height;
			monster->setPosition(area);
		}
		this->_currentStage++;
	}
}

void	SFMLunrotateDropdown::downButton(SFMLshape *)
{
	sf::Rect<int>	area;

	if (this->_objs.size() > 1 && _currentStage > 0)
	{
		for (auto &monster : this->_objs)
		{
			area = monster->getPosition();
			area.top += area.height;
			monster->setPosition(area);
		}
		this->_currentStage--;
	}
}

void	SFMLunrotateDropdown::Draw(sf::RenderWindow &window)
{
	int		count = 0;
	sf::Vector2<int>	pos		= this->_posObjs;
	std::list<SFMLshape *> list	= this->_objs;

	if (_type == LASTS)
		list.reverse();
	for (auto &obj : list)
	{
		if (count >= _currentStage && count < _nbVisible + _currentStage)
		{
			obj->setPosition(sf::Rect<int>(pos.x, pos.y, obj->getPosition().width, obj->getPosition().height));
			obj->Draw(window);
			pos.y += obj->getPosition().height * (_type == FIRSTS ? 1 : -1);
		}
		++count;
	}
}

void	SFMLunrotateDropdown::Add(SFMLshape *shape)
{
	this->_objs.push_back(shape);	
}

void	SFMLunrotateDropdown::Clear()
{
	this->_objs.clear();
}