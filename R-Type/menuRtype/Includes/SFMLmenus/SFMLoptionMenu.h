#pragma once
#include "SFMLmenu.h"
#include "SFMLbigButton.h"
#include "SFMLbasicSlider.h"
#include "SFMLbasicInput.h"
#include "SFMLcharacterInput.h"

class SFMLoptionMenu : public SFMLmenu
{
private:
	SFMLbigButton	*_confirm;
	SFMLbigButton	*_cancel;

	SFMLbasicSlider	*_backVolume;
	SFMLbasicSlider *_soundVolume;
	SFMLbasicInput	*_nickName;

	SFMLcharacterInput	*_keyUp;
	SFMLcharacterInput	*_keyLeft;
	SFMLcharacterInput	*_keyDown;
	SFMLcharacterInput	*_keyRight;
	SFMLcharacterInput	*_keyFire;

public:
	SFMLoptionMenu();
	virtual ~SFMLoptionMenu();

private:
	void	initBaseOptionMenu();
	void	initOptions();

private:
	void	savingOptions(SFMLshape	*sender);
	void	Cancel(SFMLshape *);
	void	changeBackSound(SFMLslider *);
};
