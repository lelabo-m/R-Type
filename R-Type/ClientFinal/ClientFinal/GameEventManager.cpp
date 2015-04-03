#include "GameEventManager.h"
#include "ClientManager.h"
#include "menuManager.h"

GameEventManager::GameEventManager()
{
	this->initKeyCodes();
}

GameEventManager::~GameEventManager()
{
}

void	GameEventManager::initKeyCodes()
{
	int i = 0;
	std::string vals;
	char c = 'a';

	this->_keys[-1] = "";
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

const std::string	&GameEventManager::getKeyCode(int code) const
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

void	GameEventManager::add(SFMLshape *)
{

}

void	GameEventManager::add(std::string keycode, const std::function<void(void)> &func)
{
	this->_funcsKey[keycode] = func;
}

void	GameEventManager::clear()
{
	this->_funcsKey.clear();
}

void	GameEventManager::execute(const sf::Event &ev)
{
	if (ev.type == sf::Event::Closed)
	{
		ClientManager::getInstance()->askLeaveLobby();
		menuManager::getInst()->goTo("");
	}
}

void	GameEventManager::executeKeyboard(KeyBoard *kb)
{
	if (kb)
	{
		for (const auto &kv : kb->State())
			if (kv.second)
			{
				std::string code = getKeyCode(kv.first);
				if (this->_funcsKey.find(code) != this->_funcsKey.end() && this->_funcsKey[code])
				{
					this->_funcsKey[code]();
				}
			}
	}
}