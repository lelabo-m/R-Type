#pragma once
#include "SFMLmenu.h"
#include "SFMLbasicText.h"
#include "SFMLbasicInput.h"
#include "SFMLbigButton.h"
#include "SFMLsubmenuButton.h"
#include "SFMLdropdownMultiplayer.h"
#include "TickTimer.h"

class SFMLmultiMenu :
	public SFMLmenu
{
private:
	SFMLsubmenuButton	*_mapselect;
	SFMLsubmenuButton	*_refresh;
	SFMLbigButton		*_cancel;

	SFMLdropdownMultiplayer		*_gamelisting;
	TickTimer					_timer;

public:
	SFMLmultiMenu();
	virtual ~SFMLmultiMenu();

private:
	void	initBasemultiMenu();
	void	initMultiMenu();
	void	createMapMulti(SFMLshape *);

	virtual void executeRequests();
	virtual void updateDynamicContents(const sf::Event &ev);
	virtual void displayDynaicContents();
	void	updateGames();
	void	outTimeCreateGame();
	void	Cancel(SFMLshape *);
	void	Refresh(SFMLshape *);
};

