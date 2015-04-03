#include "Monster.h"

Monster::Monster() : GameObject(ObjectType::MONSTER)
{
	_currentMoves = _patternMoves.begin();
}

Vector2D	Monster::Move()
{
	if (_currentMoves == _patternMoves.end())
		_currentMoves = _patternMoves.begin();
   Vector2D	path(*_currentMoves);
   _currentMoves++;
   return path;
}

size_t		Monster::ScoreValue() const
{
	return _scorevalue;
}
