#pragma once
#include "SFMLbasePopUp.h"
#include "SFMLbuttonLauncher.h"
#include "SFMLbigButton.h"

class SFMLstartEditorPopUp : public SFMLbasePopUp
{
private:
	SFMLbuttonLauncher		*_emptyMap;
	SFMLbuttonLauncher		*_listMap;
	SFMLbigButton			*_cancel;
	std::string				_mapEditorName;

public:
	SFMLstartEditorPopUp();
	virtual ~SFMLstartEditorPopUp();

private:
	void			initPopUp();
	virtual void	executeRequests();
	virtual	void	updateDynamicContents(const sf::Event &);

	void	editorMode(SFMLshape *);
	void	mapSelectorEditor(SFMLshape *);
	void	Cancel(SFMLshape *);
};

