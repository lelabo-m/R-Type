#include "GameMessage.h"


GameMessage::GameMessage()
{
}

GameMessage::~GameMessage()
{
}

void GameMessage::setGameID(char id)
{
	_gameID = id;
}

void GameMessage::setPlayerID(char id)
{
	_playerID = id;
}

void GameMessage::setType(char type)
{
	_type = type;
}

char GameMessage::getGameID() const
{
	return _gameID;
}

char GameMessage::getPlayerID() const
{
	return _playerID;
}

char GameMessage::getType() const
{
	return _type;
}

void GameMessage::setSessionID(unsigned int id)
{
	_sessionID = id;
}

unsigned int GameMessage::getSessionID() const
{
	return _sessionID;
}

void GameMessage::setCycleShift(int cycle)
{
	_cycleShift = cycle;
}

int GameMessage::getCycleShift() const
{
	return _cycleShift;
}