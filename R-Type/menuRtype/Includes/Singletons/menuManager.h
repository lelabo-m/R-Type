#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <string>

class menuManager
{

private:
	std::string				_previous;
	std::string 			_currentMenu;
	static menuManager	*inst;

private:
	const menuManager &operator=(const menuManager &) { return *this; };
	const menuManager &operator=(const menuManager &&) { return *this; };
	menuManager();
	~menuManager() {};

public:
	static menuManager		*getInst();
	static void				deleteInst();
	const std::string 		&getCurrentMenu() const;
	void 					goTo(const std::string &);
	void					goToPrevious();
};

#endif