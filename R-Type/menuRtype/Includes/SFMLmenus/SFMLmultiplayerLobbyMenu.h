#pragma once
#include "SFMLmenu.h"
#include "SFMLplayermulti.h"
#include "SFMLsubmenuButton.h"
#include "SFMLunrotateDropdown.h"
#include "SFMLbigButton.h"
#include "SFMLinputLauncher.h"

class SFMLmultiplayerLobbyMenu : public SFMLmenu
{

private:
	playerStatus			_status;
	sf::Text 				_MapName;
	SFMLsubmenuButton		* _launch;
	SFMLsubmenuButton		* _mapSelect;
	SFMLinputLauncher		* _chatInput;
	SFMLbigButton			* _cancel;
	SFMLunrotateDropdown	*_chat;
	SFMLunrotateDropdown	*_players;

public:
	SFMLmultiplayerLobbyMenu(playerStatus);
	virtual ~SFMLmultiplayerLobbyMenu();

private:
	void initBaseMenu();
	void initLobby();
	void initChat();
	virtual void updateDynamicContents(const sf::Event &ev);
	virtual void displayDynaicContents();
	virtual void executeRequests();
	void 	defineMap(SFMLshape *);
	void	addToChat(SFMLinput *);
	void	launchGame(SFMLshape *);
	void	Cancel(SFMLshape *);
};

