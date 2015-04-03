#ifndef _SFMLPLAYMAPSELECTORMENU_H_
# define _SFMLPLAYMAPSELECTORMENU_H_

#include "SFMLmenu.h"
#include "SFMLbigButton.h"

class SFMLplaymapSelectorMenu : public SFMLmenu
{
private:
	SFMLbigButton			*_cancel;
	SFMLbigButton			*_refresh;
	std::list<SFMLshape *>	_listMaps;

public:
	SFMLplaymapSelectorMenu();
	virtual ~SFMLplaymapSelectorMenu();

private:
	void	initBasemapSelectorMenu();
	void	initMapSelector();
	virtual void	executeRequests();
	virtual void	updateDynamicContents(const sf::Event &);
	virtual void	displayDynaicContents();
	void			Cancel(SFMLshape *);
	void			SubmitLevel(SFMLshape *);
	void			Refresh(SFMLshape *);
};

#endif