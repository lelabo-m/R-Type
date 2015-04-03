#pragma once
#include "SFMLinput.h"

class SFMLinputLauncher : public SFMLinput
{
public:
	SFMLinputLauncher(const sf::Vector2<int> &pos, int sizeMax = 30);
	virtual ~SFMLinputLauncher();

private:
	void	hoverFunc(SFMLshape *);
	void	unhoverFunc(SFMLshape *);
};

