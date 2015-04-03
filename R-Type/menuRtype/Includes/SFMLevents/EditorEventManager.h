#pragma once

#include <list>
#include "IEventManager.h"
#include "SFMLmonsterButton.h"
#include "GraphicalElem.h"
#include "SFMLmonsterDnD.h"

class EditorEventManager : public IEventManager
{
public:
	std::list<SFMLshape *>				*_shapes;
	std::list<SFMLanimation *>			*_monsters;
	std::list<SFMLmonsterDnD *>			*_wholeMonsters;
	std::map<int, std::string>			_keys;

public:
	EditorEventManager(std::list<SFMLshape *> *, std::list<SFMLanimation *> *, std::list<SFMLmonsterDnD *> *);
	virtual ~EditorEventManager();

public:
	const std::string	&getKeyCode(int) const;
	void	initKeyCodes();
	void	execute(const sf::Event &);
	void	add(SFMLshape *);

private:
	void	mouseEvents(const sf::Event &, std::list<SFMLshape *> *, int nb = -1);
	void	clickEvents(const sf::Event &, std::list<SFMLshape *> *, int nb = -1);
	void	releaseEvents(const sf::Event &, std::list<SFMLshape *> *, int nb = -1);
	void	keyPressedEvents(const sf::Event &, std::list<SFMLshape *> *, int nb = -1);
};