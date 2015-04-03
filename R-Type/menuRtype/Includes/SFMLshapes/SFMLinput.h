#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "SFMLshape.h"

class SFMLinput : public SFMLshape
{

protected:
	sf::Text		_message;
	int				_maxMessage;

public:
	bool			_textingFocus;

protected:
	SFMLinput();

public:
	SFMLinput(int, const sf::Texture &, const sf::Rect<int> &);
	SFMLinput(int, const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLinput();

public:
	std::function<void(SFMLinput *)>	_funcTextingFocus;
	std::function<void(SFMLinput *)>	_funcTextingUnfocus;
	std::function<void(SFMLinput *)>	_funcValidInput;

public:
	virtual void Draw(sf::RenderWindow &window);
	virtual void addToInput(const std::string &);
	const std::string	getMessage() const;
	void				setMessage(const std::string &);

protected:
	virtual void InitInputLabel();

private:
	virtual void InitFunctions();
};

