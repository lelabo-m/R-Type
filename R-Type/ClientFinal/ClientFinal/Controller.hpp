#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <map>
#include <list>
#include "EventManager.h"
#include "Singleton.hpp"

typedef std::map<size_t, bool>					ActionState;
typedef std::map<size_t, float>					KeyState;
typedef std::map<size_t, std::list<size_t> >	Binding;
enum class ControllerType { KEYBOARD, MOUSE, GAMEPAD, LEAP, MYO };
class IController
{
public:
	virtual ControllerType	Type() const = 0;
	virtual bool			Connected() const = 0;
	virtual void			Reconnect() const = 0;
	virtual void			Update(const sf::Event&) = 0;
	virtual bool			HasUpdate() const = 0;
	virtual bool			Bind(size_t, size_t) = 0;
	virtual ActionState&	State() = 0;
	virtual ~IController() {};
};

class BaseController : public IController
{
public:
	BaseController(ControllerType t) : _type(t), _update(false) {};
	virtual ~BaseController() {};

	ControllerType	Type() const
	{ return _type; }

	bool			HasUpdate() const
	{ return _update; }

	bool			Bind(size_t key, size_t action)
	{
		if (_keys.find(key) == _keys.end())
			return false;
		_binds[action].push_back(key);
		_actions[action] = false;
		return true;
	}

	ActionState&	State()
	{
		_update = false;
		return _actions;
	}
	
	bool			Connected() const
	{ return _connected; }

protected:
	ControllerType	_type;
	bool			_connected;
	bool			_update;
	KeyState		_keys;
	ActionState		_actions;
	Binding			_binds;
};

typedef std::list<BaseController*>			ControllerList;
typedef std::map<size_t, BaseController*>	ControllerUsed;
class ControllerManager : public Singleton<ControllerManager>
{
public:
	ControllerManager()
	{
		instance = this;
	}
	~ControllerManager()
	{
		_controllers.clear();
		_used.clear();
		instance = nullptr;
	}
	void			AddController(BaseController* c)
	{
		_controllers.push_back(c);
	}
	void			Update(const sf::Event& e)
	{
		for (auto controller : _controllers)
		{
			if (controller->Connected())
				controller->Update(e);
			else
				controller->Reconnect();
		}
	}
	ControllerList	Controllers()
	{
		return _controllers;
	}
	ControllerList	ControllersByType(ControllerType type)
	{
		ControllerList	list;
		for (auto controller : _controllers)
		{
			if (controller->Type() == type)
				list.push_back(controller);
		}
		return list;
	}
	ControllerList	NotUsed()
	{
		ControllerList	list;
		for (auto controller : _controllers)
		{
			bool isused = false;
			for (auto user : _used)
			{
				if (controller == user.second)
					isused = true;
			}
			if (isused == false)
				list.push_back(controller);
		}
		return list;
	}
	ControllerList	NotUsedByType(ControllerType type)
	{
		ControllerList	list;
		for (auto controller : _controllers)
		{
			bool isused = false;
			for (auto user : _used)
			{
				if (controller == user.second)
					isused = true;
			}
			if (isused == false && controller->Type() == type)
				list.push_back(controller);
		}
		return list;
	}
	void			ControllerAttribution(size_t player, BaseController* controller)
	{
		_used[player] = controller;
	}
	BaseController*	Controller(size_t player)
	{
		if (_used.find(player) != _used.end())
			return _used[player];
		return nullptr;
	}
private:
	ControllerList	_controllers;
	ControllerUsed	_used;
};

enum KeyAdvanced { CTRL = sf::Keyboard::KeyCount, ALT, SHIFT, SYSTEM };

class KeyBoard : public BaseController
{
public:
	KeyBoard();
	void	Update(const sf::Event&);
	bool	Connected() const;
	void	Reconnect() const;
};

enum GamePadAxis { LEFT_X = sf::Joystick::ButtonCount + 0, LEFT_Y, RIGHT_X, RIGHT_Y, ARROWS_X = sf::Joystick::ButtonCount + 6, ARROWS_Y };

class GamePad : public BaseController
{
private:
	int _id;
public:
	GamePad(void);
	void Update(const sf::Event &);
	bool Connected(void) const;
	void Reconnect(void) const;
};

#endif /* !CONTROLLER_H_ */