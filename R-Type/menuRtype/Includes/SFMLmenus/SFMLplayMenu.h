#pragma once
#include "SFMLmenu.h"
#include "SFMLlittleButton.h"

class SFMLplayMenu : public SFMLmenu
{
private:
	SFMLlittleButton	*_single;
	SFMLlittleButton	*_multi;
	SFMLlittleButton	*_editor;
	SFMLlittleButton	*_cancel;

public:
	SFMLplayMenu();
	virtual ~SFMLplayMenu();
	
private:
	void	initPlayMenu();
	void	Single(SFMLshape *);
	void	Multi(SFMLshape *);
	void	Editor(SFMLshape *);
	void	Cancel(SFMLshape *);
	void	outTimeCreateGame();
};

