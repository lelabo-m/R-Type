#pragma once
#include <list>
#include "SFMLbaseEditor.h"
#include "SFMLeditormenuButton.h"
#include "SFMLmonsterButton.h"
#include "SFMLslider.h"

class SFMLeditor : public SFMLbaseEditor
{
private:
	SFMLeditormenuButton	*_save;
	SFMLeditormenuButton	*_length;
	SFMLeditormenuButton	*_quit;

private:
	SFMLbutton				*_up;
	SFMLbutton				*_down;

private:
	SFMLbutton				*_play;
	SFMLbutton				*_pause;
	SFMLbutton				*_stop;
	SFMLslider				*_timeline;
	bool					_isPlaying;

private:
	std::string				_mapName;

public:
	SFMLeditor(const std::string &);
	virtual ~SFMLeditor();

private:
	void	initEditor();
	void	initSpriteList();
	void	initMenu();
	void	initTimer();
	void	initMonsterList();

private:
	void	upButton(SFMLshape *);
	void	downButton(SFMLshape *);

public:
	void			createMonster(SFMLshape *);


private:
	virtual void	executeRequests();
	virtual void	displayDynamicContents();
	virtual void	updateDynamicContents(const sf::Event &);
	virtual void	updateContent();

	void	play(SFMLshape *);
	void	pause(SFMLshape *);
	void	stop(SFMLshape *);
	void	setTimer(SFMLslider *);
	void	defineLengthLevel(SFMLshape *);
	void	saveFile(SFMLshape *);
	void	Quit(SFMLshape *);
};