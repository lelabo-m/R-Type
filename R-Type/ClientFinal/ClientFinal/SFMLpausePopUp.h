#pragma once
#include "SFMLbasePopUp.h"
#include "SFMLbuttonLauncher.h"

class SFMLpausePopUp : public SFMLbasePopUp
{
private:
	SFMLbuttonLauncher	*_unpause;
	SFMLbuttonLauncher	*_quit;

private:
	void	initPopUp();
	void	unpause(SFMLshape *);
	void	quit(SFMLshape *);

public:
	SFMLpausePopUp();
	virtual ~SFMLpausePopUp();
};

