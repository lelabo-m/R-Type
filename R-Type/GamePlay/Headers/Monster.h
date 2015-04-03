#ifndef MONSTER_H_
#define MONSTER_H_

#include "GameObject.h"

class IMonster
{
public:
	virtual std::vector<Bullet*>	Fire() = 0;
	virtual ~IMonster() {};
};

class Monster : public GameObject, public IMonster
{
public:
	Monster();
	Vector2D						Move();
	size_t							ScoreValue() const;
protected:
	std::list<Vector2D>::iterator	_currentMoves;
	std::list<Vector2D>				_patternMoves;
	size_t							_scorevalue;
};

#endif /* !MONSTER_H_ */