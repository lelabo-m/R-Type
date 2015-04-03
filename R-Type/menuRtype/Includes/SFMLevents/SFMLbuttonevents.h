#pragma once

#include "SFMLbutton.h"


class SFMLbuttonevents
{
public:
	SFMLbuttonevents();
	~SFMLbuttonevents();

public:
	static	void littleButtonHover(SFMLshape *);
	static	void littleButtonUnhover(SFMLshape *);
	static 	void littleButtonClick(SFMLshape *);

	static void bigButtonHover(SFMLshape *);
	static void bigButtonUnhover(SFMLshape *);
	static void bigButtonClick(SFMLshape *);

	static void	mapselectorButtonHover(SFMLshape *);
	static void	mapselectorButtonUnhover(SFMLshape *);
	static void	mapselectorButtonClick(SFMLshape *);

	static void	submenuButtonHover(SFMLshape *);
	static void	submenuButtonUnhover(SFMLshape *);
	static void	submenuButtonClick(SFMLshape *);

	static void	editorButtonHover(SFMLshape *);
	static void editorButtonUnhover(SFMLshape *);

	static	void optionCancel(SFMLshape *);
	static	void levelScore(SFMLshape *);
	static	void submitLevel(SFMLshape *);
	static	void selectMapMulti(SFMLshape *);

	static	void upArrowHover(SFMLshape *);
	static	void upArrowUnhover(SFMLshape *);
	static	void downArrowHover(SFMLshape *);
	static	void downArrowUnhover(SFMLshape *);

	static	void playButtonHover(SFMLshape *);
	static	void playButtonUnhover(SFMLshape *);
	static	void pauseButtonHover(SFMLshape *);
	static	void pauseButtonUnhover(SFMLshape *);
	static	void stopButtonHover(SFMLshape *);
	static	void stopButtonUnhover(SFMLshape *);
	static	void monsterButtonHover(SFMLshape *);
	static	void monsterButtonUnhover(SFMLshape *);
};

