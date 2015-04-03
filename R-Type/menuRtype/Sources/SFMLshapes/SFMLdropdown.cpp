#include <iostream>
#include "SFMLdropdown.h"
#include "databaseSFML.h"

SFMLdropdown::SFMLdropdown(int nbVisible)
{
	this->_nbVisible = nbVisible;
	this->initDropdown();
}

SFMLdropdown::SFMLdropdown(const sf::Texture &upButton, const sf::Vector2<int> &posUp,
							const sf::Texture &downButton,	const sf::Vector2<int> &posDown,
							const sf::Vector2<int> &posObjs, int nbVisible)
{
	_up = new SFMLbutton("", upButton, posUp);
	_down = new SFMLbutton("", downButton, posDown);
	_posObjs = posObjs;
	
	_up->_funcRelease = std::bind(&SFMLdropdown::upButton, this, _up);
	_down->_funcRelease = std::bind(&SFMLdropdown::downButton, this, _down);
	this->_nbVisible = nbVisible;
}

SFMLdropdown::~SFMLdropdown()
{
	delete _up;
	delete _down;
	_objs.clear();
}

void	SFMLdropdown::initDropdown()
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_up = new SFMLbutton("", db->getTexture("upArrow"), sf::Vector2<int>(0, 0));
	_down = new SFMLbutton("", db->getTexture("downArrow"), sf::Vector2<int>(0, 0));

	_up->_funcRelease	= std::bind(&SFMLdropdown::upButton, this, _up);
	_down->_funcRelease	= std::bind(&SFMLdropdown::downButton, this, _down);
}

void	SFMLdropdown::registerTo(EventManager &event, std::list<SFMLshape *> &list)
{
	std::cout << "Adding listing" << std::endl;
	list.push_back(_up);
	list.push_back(_down);

	event.add(_up);
	event.add(_down);
}

void	SFMLdropdown::Draw(sf::RenderWindow &window)
{
	int		count = 0;
	sf::Vector2<int>	pos = this->_posObjs;

	for (auto &obj : this->_objs)
	{
		obj->setPosition(sf::Rect<int>(pos.x, pos.y, obj->getPosition().width, obj->getPosition().height));
		if (count < this->_nbVisible)
			obj->Draw(window);
		pos.y += obj->getPosition().height;
		count++;
	}
}

void	SFMLdropdown::upButton(SFMLshape *)
{
	SFMLshape		*subject;
	sf::Rect<int>	area;


	std::cout << "Classic up button" << std::endl;
	if (this->_objs.size() > 1)
	{
		subject = this->_objs.front();
		subject->setPosition(this->_objs.back()->getPosition());

		this->_objs.pop_front();
		for (auto &monster : this->_objs)
		{
			area = monster->getPosition();
			area.top -= area.height;
			monster->setPosition(area);
		}
		this->_objs.push_back(subject);
	}
}

void	SFMLdropdown::downButton(SFMLshape *)
{
	SFMLshape	*subject;
	sf::Rect<int>	area;

	if (this->_objs.size() > 1)
	{
		subject = this->_objs.back();
		subject->setPosition(this->_objs.front()->getPosition());

		this->_objs.pop_back();
		for (auto &monster : this->_objs)
		{
			area = monster->getPosition();
			area.top += area.height;
			monster->setPosition(area);
		}
		this->_objs.push_front(subject);
	}
}

void	SFMLdropdown::Add(SFMLshape *shape)
{
	this->_objs.push_back(shape);
}

void	SFMLdropdown::Clear()
{
	this->_objs.clear();
}

std::list<SFMLshape *>	&SFMLdropdown::getList()
{
	return this->_objs;
}
