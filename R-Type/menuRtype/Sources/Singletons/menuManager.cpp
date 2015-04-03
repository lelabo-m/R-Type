#include "menuManager.h"

menuManager *menuManager::inst = NULL;

menuManager::menuManager()
{
	_currentMenu = "Launcher";
	_previous = "";
}

menuManager *menuManager::getInst()
{
	if (inst == NULL)
		inst = new menuManager();
	return inst;
}

void menuManager::deleteInst()
{
	if (inst)
	{
		delete inst;
	}
	inst = NULL;
}

const std::string 		&menuManager::getCurrentMenu() const
{
	return this->_currentMenu;
}

void 					menuManager::goTo(const std::string &menu)
{
	this->_previous = _currentMenu;
	this->_currentMenu = menu;
}

void					menuManager::goToPrevious()
{
	std::string			tmp = this->_currentMenu;

	this->_currentMenu = this->_previous;
	this->_previous = tmp;
}