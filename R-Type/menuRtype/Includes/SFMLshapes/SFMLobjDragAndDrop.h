#pragma once
#include "SFMLshape.h"

class SFMLobjDragAndDrop : public SFMLshape
{
public:
	bool	_click;

protected:
	SFMLobjDragAndDrop();

public:
	SFMLobjDragAndDrop(const sf::Texture &, const sf::Vector2<int> &);
	virtual ~SFMLobjDragAndDrop();
	const SFMLobjDragAndDrop &operator=(const SFMLobjDragAndDrop &);
	const SFMLobjDragAndDrop &operator=(const SFMLobjDragAndDrop &&);
	virtual void	changingPosition(int, int);

private:
	virtual void	initFunctions();

public:
	std::function<void(SFMLshape *)>	_funcPositionChanged;
};