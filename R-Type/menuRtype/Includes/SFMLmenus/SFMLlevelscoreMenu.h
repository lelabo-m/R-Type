#pragma once
#include "SFMLmenu.h"
#include "SFMLbigButton.h"
#include "SFMLscore.h"

class SFMLlevelscoreMenu : public SFMLmenu
{
private:
	SFMLbigButton				*_cancel;
	std::list<SFMLscore>		_bestScores;

public:
	SFMLlevelscoreMenu(const std::string &levelName="");
	~SFMLlevelscoreMenu();

private:
	void initBaseLevelscoreMenu(const std::string &);
	void Cancel(SFMLshape *);
	virtual void executeRequests();
	virtual void displayDynaicContents();
};

