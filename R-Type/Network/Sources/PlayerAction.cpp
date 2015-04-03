#include "PlayerAction.h"


PlayerAction::PlayerAction(char action) : _action(action)
{
}


PlayerAction::~PlayerAction()
{
}

char PlayerAction::getAction() const
{
	return _action;
}

void PlayerAction::setAction(char action)
{
	_action = action;
}