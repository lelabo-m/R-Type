#pragma once
#include "SFMLmenu.h"
#include "SFMLlittleButton.h"

class SFMLmainMenu : public SFMLmenu
{
private:
	SFMLlittleButton	*_play;
	SFMLlittleButton	*_option;
	SFMLlittleButton	*_scores;
	SFMLlittleButton	*_exit;

public:
	SFMLmainMenu();
	virtual ~SFMLmainMenu();

private:
	void	initBaseMainMenu();
	void	Cancel(SFMLshape *);
	void	Play(SFMLshape *);
	void	Scores(SFMLshape *);
	void	Options(SFMLshape *);
};

