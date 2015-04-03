#include <iostream>
#include <SFML/System.hpp>
#include "EventManager.h"
#include "SFMLbutton.h"
#include "SFMLslider.h"
#include "SFMLobjDragAndDrop.h"
#include "SFMLinput.h"
#include "menuManager.h"
#include "Controller.hpp"

EventManager::EventManager()
{
	this->initKeyCodes();
}

EventManager::EventManager(const std::list<SFMLshape *> &othlist)
{
	this->_forms = othlist;
	this->initKeyCodes();
}

EventManager::~EventManager()
{
	this->_keys.clear();
	this->_forms.clear();
}

void	EventManager::initKeyCodes()
{
	int i = 0;
	std::string vals;
	char c = 'a';

	this->_keys[-1] = ".";
	for (i = 0, c = 'a'; c <= 'z'; i++, c++)
		this->_keys[i] = std::string(1, c);

	for (i = 102, c = 'A'; c <= 'Z'; i++, c++)
		this->_keys[i] = std::string(1, c);

	for (i = 75, c = '0'; c <= '9'; i++, c++)
		this->_keys[i] = std::string(1, c);

	for (i = 128, c = '0'; c <= '9'; i++, c++)
		this->_keys[i] = std::string(1, c);

	for (i = 26, vals = "!@#$%^&*()"; i <= 35; i++)
		this->_keys[i] = std::string(1, vals[i - 26]);

	for (i = 46, vals = "-[],.²/';="; i <= 55; i++)
		this->_keys[i] = std::string(1, vals[i - 46]);

	for (i = 138, vals = "_{}<> ?\":+"; i <= 147; i++)
		this->_keys[i] = std::string(1, vals[i - 138]);

	this->_keys[36] = "Esc";
	this->_keys[57] = " ";
	this->_keys[58] = "Enter";
	this->_keys[59] = "Suppr";
	this->_keys[71] = "Left";
	this->_keys[72] = "Right";
	this->_keys[73] = "Up";
	this->_keys[74] = "Down";
}

void	EventManager::add(SFMLshape *b)
{
	this->_forms.push_back(b);
}

void	EventManager::clear()
{
	this->_forms.clear();
}

const std::string	&EventManager::getKeyCode(int code) const
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

void	EventManager::mouseEvents(const sf::Event &event, std::list<SFMLshape *> &list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseMoved)
	{
		for (auto &form : list)
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

			count++;
		}
	}
}

void	EventManager::clickEvents(const sf::Event &event, std::list<SFMLshape *> &list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		for (auto &form : list)
		{
			if (nb != -1 && count >= nb) break;
			if (form->isFocus())
			{
				if (SFMLslider *slide = dynamic_cast<SFMLslider *>(form))
					slide->_click = true;
				else if (SFMLinput *input = dynamic_cast<SFMLinput *>(form))
				{
					if (!input->_textingFocus)
						if (input->_funcTextingFocus) input->_funcTextingFocus(input);
					input->_textingFocus = true;
				}
				if (form->_funcClick) form->_funcClick(form);
			}

			count++;
		}
	}
}

void	EventManager::releaseEvents(const sf::Event &event, std::list<SFMLshape *> &list, int nb)
{
	int count = 0;

	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		for (auto &form : list)
		{
			if (nb != -1 && count >= nb) break;
			if (form->isFocus())
			{
				if (form->_funcRelease) form->_funcRelease(form);
			}
			else
			{
				if (SFMLinput *input = dynamic_cast<SFMLinput *>(form))
				{
					if (input->_textingFocus)
						if (input->_funcTextingUnfocus) input->_funcTextingUnfocus(input);
					input->_textingFocus = false;
				}
			}
			if (SFMLslider *slide = dynamic_cast<SFMLslider *>(form))
				slide->_click = false;
			count++;
		}
	}
}

void	EventManager::keyEvents(const sf::Event &event, std::list<SFMLshape *> &list, int nb)
{
	int	count = 0;
	ControllerManager *cm = ControllerManager::GetInstance();

	if (event.type == sf::Event::KeyPressed)
	{
		ControllerList cl = cm->ControllersByType(ControllerType::KEYBOARD);
		KeyBoard *kb = dynamic_cast<KeyBoard *>(cl.front());

		for (auto &form : list)
		{
			if (nb != -1 && count >= nb) break;
			if (SFMLinput *input = dynamic_cast<SFMLinput *>(form))
			{
				if (kb && input->_textingFocus)
				{
					int val = -1;
					for (const auto &kv : kb->State())
						if (kv.second)
							val = kv.first;

					input->addToInput(this->getKeyCode(val));
				}
			}
			count++;
		}
	}
}

void	EventManager::execute(const sf::Event &event)
{
	if (event.type == sf::Event::Closed)
		menuManager::getInst()->goTo("");

	this->mouseEvents(event, this->_forms);
	this->releaseEvents(event, this->_forms);
	this->clickEvents(event, this->_forms);
	this->keyEvents(event, this->_forms);

	ControllerManager*	cm = ControllerManager::GetInstance();
	if (cm)
		cm->Update(event);
}

void	EventManager::execute(const sf::Event &event, std::list<SFMLshape *> &list, int nums)
{
	this->mouseEvents(event, list, nums);
	this->releaseEvents(event, list, nums);
	this->clickEvents(event, list, nums);
	this->keyEvents(event, list, nums);

	ControllerManager*	cm = ControllerManager::GetInstance();
	if (cm)
		cm->Update(event);
}
