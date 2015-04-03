#pragma once

#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "IEventManager.h"

class EventManager : public IEventManager
{
private:
	std::list<SFMLshape *>		_forms;
	std::map<int, std::string>	_keys;

public:
	EventManager();
	EventManager(const std::list<SFMLshape *> &);
	~EventManager();

public:
	void	add(SFMLshape *);
	void	clear();
	void	execute(const sf::Event &);
	void	execute(const sf::Event &, std::list<SFMLshape *> &, int nums = -1);

private:
	void				initKeyCodes();
	const std::string 	&getKeyCode(int) const;

private:
	void	mouseEvents(const sf::Event &, std::list<SFMLshape *> &, int nb = -1);
	void	clickEvents(const sf::Event &, std::list<SFMLshape *> &, int nb = -1);
	void	releaseEvents(const sf::Event &, std::list<SFMLshape *> &, int nb = -1);
	void	keyEvents(const sf::Event &, std::list<SFMLshape *> &, int nb = -1);
};

