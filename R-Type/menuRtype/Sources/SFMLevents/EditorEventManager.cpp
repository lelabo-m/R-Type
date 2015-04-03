#include "EditorEventManager.h"
#include "SFMLslider.h"
#include "SFMLobjDragAndDrop.h"
#include "SFMLinput.h"
#include "menuManager.h"

EditorEventManager::EditorEventManager(std::list<SFMLshape *> *lShapes, std::list<SFMLanimation *> *lMonsters, std::list<SFMLmonsterDnD *> *monsters)
{
	this->_shapes			= lShapes;
	this->_monsters			= lMonsters;
	this->_wholeMonsters	= monsters;

	this->initKeyCodes();
}

EditorEventManager::~EditorEventManager()
{
}

void	EditorEventManager::initKeyCodes()
{
	this->_keys[-1] = "";
	this->_keys[66] = "Suppr";
}

const std::string	&EditorEventManager::getKeyCode(int code) const
{
	try
	{
		return this->_keys.at(code);
	}
	catch (const std::out_of_range &)
	{
		return this->_keys.at(-1);
	}
}


void	EditorEventManager::add(SFMLshape *shape)
{
	this->_shapes->push_back(shape);
}

void	EditorEventManager::mouseEvents(const sf::Event &event, std::list<SFMLshape *> *list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseMoved)
	{
		for (auto &form : *list)
		{
			if (nb != -1 && count >= nb) break;
			if (form->isFocusChanged(event.mouseMove.x, event.mouseMove.y))
			{
				if (form->isFocus())
				{
					if (form->_funcFocus) form->_funcFocus(form);
				}
				else
					if (form->_funcUnfocus) form->_funcUnfocus(form);
			}
			if (SFMLslider *slide = dynamic_cast<SFMLslider *>(form))
			{
				if (slide->_click)
					slide->changingValue(event.mouseMove.x);
			}
			else if (SFMLobjDragAndDrop *dNd = dynamic_cast<SFMLobjDragAndDrop *>(form))
			{
				if (dNd->_click)
				{
					dNd->changingPosition(event.mouseMove.x, event.mouseMove.y);
					break;
				}
			}
			++count;
		}
	}
}

void	EditorEventManager::clickEvents(const sf::Event &event, std::list<SFMLshape *> *list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		for (auto &form : *list)
		{
			if (nb != -1 && count >= nb) break;
			if (form->isFocus())
			{
				if (SFMLslider *slide = dynamic_cast<SFMLslider *>(form))
					slide->_click = true;
				else if (SFMLobjDragAndDrop *dNd = dynamic_cast<SFMLobjDragAndDrop *>(form))
					dNd->_click = true;
				if (form->_funcClick) form->_funcClick(form);
			}
		}
		++count;
	}
}

void	EditorEventManager::releaseEvents(const sf::Event &event, std::list<SFMLshape *> *list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		for (auto &form : *list)
		{
			if (nb != -1 && count >= nb) break;
			if (form->isFocus())
			{
				if (form->_funcRelease) form->_funcRelease(form);
			}
			if (SFMLmonsterDnD *monsterDnD = dynamic_cast<SFMLmonsterDnD *>(form))
				monsterDnD->_textingFocus = form->isFocus();
			if (SFMLslider *slide = dynamic_cast<SFMLslider *>(form))
				slide->_click = false;
			else if (SFMLobjDragAndDrop *dNd = dynamic_cast<SFMLobjDragAndDrop *>(form))
				dNd->_click = false;
		}
		++count;
	}
}

void	EditorEventManager::keyPressedEvents(const sf::Event &event, std::list<SFMLshape *> *list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::KeyPressed)
	{
		for (auto &form : *list)
		{
			if (nb != -1 && count >= nb) break;
			if (SFMLmonsterDnD *input = dynamic_cast<SFMLmonsterDnD *>(form))
			{
				if (input->_textingFocus)
					input->addToInput(this->getKeyCode(event.key.code));
			}
			++count;
		}
	}
}

void	EditorEventManager::execute(const sf::Event &event)
{
	if (event.type == sf::Event::Closed)
		menuManager::getInst()->goTo("");

	this->mouseEvents(event, this->_shapes);
	this->mouseEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_monsters), 5);
	this->mouseEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_wholeMonsters));
	this->releaseEvents(event, this->_shapes);
	this->releaseEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_monsters), 5);
	this->releaseEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_wholeMonsters));
	this->clickEvents(event, this->_shapes);
	this->clickEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_monsters), 5);
	this->clickEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_wholeMonsters));
	this->keyPressedEvents(event, reinterpret_cast<std::list<SFMLshape *> *>(this->_wholeMonsters));
}
