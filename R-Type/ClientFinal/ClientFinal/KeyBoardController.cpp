#include "Controller.hpp"

KeyBoard::KeyBoard() : BaseController(ControllerType::KEYBOARD)
{
	for (size_t i = (sf::Keyboard::Key::Unknown + 1); i < sf::Keyboard::KeyCount; ++i)
		_keys[i] = 0.0f;
	_keys[KeyAdvanced::CTRL] = 0.0f;
	_keys[KeyAdvanced::ALT] = 0.0f;
	_keys[KeyAdvanced::SHIFT] = 0.0f;
	_keys[KeyAdvanced::SYSTEM] = 0.0f;
}

void	KeyBoard::Update(const sf::Event& event)
{
	bool isupdate = false;
	if (event.type == sf::Event::KeyPressed)
	{
		for (int i = (sf::Keyboard::Key::Unknown + 1); i < sf::Keyboard::KeyCount; ++i)
			if (event.key.code == i && _keys[i] != 1.0f)
			{
				isupdate = true;
				_keys[i] = 1.0f;
			}
		if (event.key.alt && _keys[KeyAdvanced::ALT] != 1.0f)
		{
			_keys[KeyAdvanced::ALT] = 1.0f;
			isupdate = true;
		}
		if (event.key.control  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::CTRL] = 1.0f;
			isupdate = true;
		}
		if (event.key.system  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::SYSTEM] = 1.0f;
			isupdate = true;
		}
		if (event.key.shift  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::SHIFT] = 1.0f;
			isupdate = true;
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		for (int i = (sf::Keyboard::Key::Unknown + 1); i < sf::Keyboard::KeyCount; ++i)
			if (event.key.code == i && _keys[i] != 0.0f)
			{
				_keys[i] = 0.0f;
				isupdate = true;
			}
		if (event.key.alt && _keys[KeyAdvanced::ALT] != 0.0f)
		{
			_keys[KeyAdvanced::ALT] = 0.0f;
			isupdate = true;
		}
		if (event.key.control  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::CTRL] = 0.0f;
			isupdate = true;
		}
		if (event.key.system  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::SYSTEM] = 0.0f;
			isupdate = true;
		}
		if (event.key.shift  && _keys[KeyAdvanced::CTRL] != 1.0f)
		{
			_keys[KeyAdvanced::SHIFT] = 0.0f;
			isupdate = true;
		}
	}
	if (isupdate)
	{
		for (auto action : _binds)
		{
			bool isok = true;
			for (auto key : action.second)
			{
				if (_keys[key] == 0.0f)
					isok = false;
			}
			if (_actions[action.first] != isok)
				_update = true;
			_actions[action.first] = isok;
		}
	}
}

bool	KeyBoard::Connected() const
{
	return true;
}

void	KeyBoard::Reconnect() const
{
}