#pragma once

#include <list>
#include <map>
#include <string>
#include "IEventManager.h"
#include "Controller.hpp"

class GameEventManager : public IEventManager
{
public:
	GameEventManager();
	virtual ~GameEventManager();

protected:
	std::map<std::string, std::function<void(void)> >	_funcsKey;
	std::map<int, std::string>							_keys;

protected:
	void				initKeyCodes();
	const std::string 	&getKeyCode(int) const;

public:
	void	add(SFMLshape *);
	void	add(std::string, const std::function<void(void)> &);
	void	clear();
	void	execute(const sf::Event &);
	void	executeKeyboard(KeyBoard *);
};

