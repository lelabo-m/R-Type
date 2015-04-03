#pragma once
#include "SFMLinput.h"
#include "gameOptions.h"

class SFMLcharacterInput : public SFMLinput
{
protected:
	SFMLcharacterInput();

protected:
	keysOpt		_key;

public:
	SFMLcharacterInput(const sf::Vector2<int> &, keysOpt, const std::string &txt = "");
	virtual ~SFMLcharacterInput();

public:
	virtual void addToInput(const std::string &);
};

