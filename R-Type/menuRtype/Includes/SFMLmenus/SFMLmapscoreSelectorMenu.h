#pragma once
#include "SFMLmenu.h"
#include "SFMLbigButton.h"

class SFMLmapscoreSelectorMenu : public SFMLmenu
{
private:
	SFMLbigButton			*_cancel;
	std::list<SFMLshape *>	_listMaps;

public:
	SFMLmapscoreSelectorMenu();
	virtual ~SFMLmapscoreSelectorMenu();

private:
	void	initBaseMapscoreselectorMenu();
	void	createLevelButtons();
	virtual void updateDynamicContents(const sf::Event &ev);
	virtual void displayDynaicContents();
};

