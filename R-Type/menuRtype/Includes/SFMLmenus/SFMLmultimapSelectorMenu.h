#pragma once
#include "SFMLmenu.h"
#include "SFMLbigButton.h"

class SFMLmultimapSelectorMenu : public SFMLmenu
{
private:
	SFMLbigButton	*_cancel;

public:
	SFMLmultimapSelectorMenu();
	virtual ~SFMLmultimapSelectorMenu();

private:
	void	initBasemultiMapselector();
	void	initMultiMapselector();
};

