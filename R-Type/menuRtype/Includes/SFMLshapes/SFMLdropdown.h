#pragma once

#include <list>
#include "SFMLshape.h"
#include "SFMLbutton.h"
#include "EventManager.h"

class SFMLdropdown : public SFMLshape
{
public:
	SFMLbutton	*_up;
	SFMLbutton	*_down;

protected:
	int							_nbVisible;
	sf::Vector2<int>			_posObjs;
	std::list<SFMLshape *>		_objs;

protected:
	SFMLdropdown(int nbVisible = 0);
	SFMLdropdown(	const sf::Texture &, const sf::Vector2<int> &,
					const sf::Texture &, const sf::Vector2<int> &,
					const sf::Vector2<int> &, int nbVisible = 0);
	virtual ~SFMLdropdown();

public:
	void	registerTo(EventManager &, std::list<SFMLshape *> &);

private:
	void	upButton(SFMLshape *);
	void	downButton(SFMLshape *);
	void	initDropdown();

public:
	virtual void Draw(sf::RenderWindow &);
	virtual void Add(SFMLshape *);
	virtual void Clear();
	std::list<SFMLshape *>	&getList();
};

