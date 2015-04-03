#pragma once
#include <iostream>
#include "SFMLobjDragAndDrop.h"
#include "SFMLinput.h"
#include "Timer.h"

class SFMLmonsterDnD : public SFMLobjDragAndDrop
{
public:
	SFMLmonsterDnD(const sf::Texture &, const std::string &, const sf::Vector2<int> &, const Time &);
	virtual ~SFMLmonsterDnD();

public:
	bool			_textingFocus;

protected:
	bool				_toDelete;
	bool				_isVisible;
	sf::Vector2<int>	_spawnPos;
	Time				_spawnTimer;
	std::string			_name;

public:
	void					addToInput(const std::string &);
	virtual void			changingPosition(int, int);
	bool					hasToDelete() const;
	bool					isVisible() const;
	void					checkVisibility(const Time &);
	void					updateXPosition(const Time &);
	const Time &			getTimer() const;
	const sf::Vector2<int>	&getSpawnPos() const;
	const std::string		&getName() const;
};