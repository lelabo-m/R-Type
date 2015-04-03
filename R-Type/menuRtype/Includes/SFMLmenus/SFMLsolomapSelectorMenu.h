#pragma once
#include "SFMLmenu.h"
#include "SFMLbigButton.h"

class SFMLsolomapSelectorMenu : public SFMLmenu
{
private:
	SFMLbigButton	*_cancel;

public:
	SFMLsolomapSelectorMenu();
	virtual ~SFMLsolomapSelectorMenu();

private:
	void	initSoloMenu();
	void	initMapselector();
};

