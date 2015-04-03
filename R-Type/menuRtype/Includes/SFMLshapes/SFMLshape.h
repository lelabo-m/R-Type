#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class SFMLshape
{
protected:
	sf::Rect<int>	_area;
	bool			_focus;

public:
	sf::Sprite		_background;

protected:
	SFMLshape();

public:
	SFMLshape(const sf::Texture &, const sf::Rect<int> &);
	SFMLshape(const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLshape();
	const SFMLshape &operator=(const SFMLshape &);
	const SFMLshape &operator=(const SFMLshape &&);

public:
	bool isFocusChanged(const sf::Vector2<int> &);
	bool isFocusChanged(int, int);
	bool isFocus() const;
	virtual void setPosition(const sf::Rect<int> &);
	virtual const sf::Rect<int>	&getPosition() const;

public:
	virtual void Draw(sf::RenderWindow &window);
	std::function<void(SFMLshape *)>	_funcFocus;
	std::function<void(SFMLshape *)>	_funcUnfocus;
	std::function<void(SFMLshape *)>	_funcClick;
	std::function<void(SFMLshape *)>	_funcRelease;

protected:
	void InitForm();
	virtual void InitFunctions();
};

