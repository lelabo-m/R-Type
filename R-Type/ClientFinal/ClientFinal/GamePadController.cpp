#include <iostream>
#include <iomanip>
#include "Controller.hpp"

GamePad::GamePad(void) : BaseController(ControllerType::GAMEPAD)
{
	static int idx;

	this->_id = idx;
	for (size_t i = 0; i < sf::Joystick::Count; i++)
		this->_keys[i] = 0.0f;
	this->_keys[GamePadAxis::LEFT_X] = 0.0f;
	this->_keys[GamePadAxis::LEFT_Y] = 0.0f;
	this->_keys[GamePadAxis::RIGHT_X] = 0.0f;
	this->_keys[GamePadAxis::RIGHT_Y] = 0.0f;
	this->_keys[GamePadAxis::ARROWS_X] = 0.0f;
	this->_keys[GamePadAxis::ARROWS_Y] = 0.0f;
	idx++;
}

void GamePad::Update(const sf::Event &event)
{
	//bool isupdate = false;

	// Buttons
	if (event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased)
		this->_keys[event.joystickButton.button] = sf::Joystick::isButtonPressed(this->_id, event.joystickButton.button);
	
	// Joysticks
	if (event.type == sf::Event::JoystickMoved)
	{
		this->_keys[sf::Joystick::ButtonCount + event.joystickMove.axis] = 
			static_cast<float>(static_cast<int>(sf::Joystick::getAxisPosition(this->_id, static_cast<sf::Joystick::Axis>(event.joystickMove.axis))));
	}
}

bool GamePad::Connected(void) const
{
	return sf::Joystick::isConnected(this->_id);
}

void GamePad::Reconnect(void) const
{
}