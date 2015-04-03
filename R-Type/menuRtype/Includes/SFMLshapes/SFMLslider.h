#pragma once

#include <functional>
#include "SFMLshape.h"

class SFMLslider : public SFMLshape
{
public:
	sf::Sprite		_tick;
	bool			_click;

protected:
	int		_value;
	int		_maxValue;
	int		_minValue;

public:
	std::function<void(SFMLslider *)>	_funcValueChanged;

protected:
	SFMLslider();

public:
	SFMLslider(int, int, const sf::Texture &, const sf::Texture &, const sf::Vector2<int> &);
	SFMLslider(int, int, int, const sf::Texture &, const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLslider();
	const SFMLslider &operator=(const SFMLslider &);
	const SFMLslider &operator=(const SFMLslider &&);

public:
	virtual void Draw(sf::RenderWindow &window);

public:
	void	changingValue(int);
	void	updateSlider(int);
	int		getValue() const;

public:
	void	setMaxValue(int);
	void	setMinValue(int);

protected:
	void InitSlider();

private:
	void InitFunctions();
};

