#pragma once
#include <SFML/Graphics.hpp>
#include "SFMLshape.h"

class IEventManager
{
public:
	IEventManager() {};
	virtual ~IEventManager() {};

public:
	virtual void execute(const sf::Event &) = 0;
	virtual void add(SFMLshape *) = 0;
};

