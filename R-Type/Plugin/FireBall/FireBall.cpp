#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "GameContainer.h"
#include "FireBall.h"
#include "FileSystem.hpp"
#include "ObjectRepresentation.h"

// ##### FireBallMonster #####
FireBallMonster::FireBallMonster() : Monster()
{
	_name = "FireBallMonster";
	_life = 1000;
	_scorevalue = 1500;
	_patternMoves.push_back(Vector2D(3, 0));
}

void					FireBallMonster::Init(GameContext *gc)
{
	_type = ObjectType::OTHER;
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(17, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	gc->Contain().Register(this);
	gc->Contain().EnableCollision(this);
}

void					FireBallMonster::Destroy()
{
	if (!this->IsDestroyable())
		return;
	if (!_gc)
		return;
	_gc->Contain().Unregister(this);
	_gc->Contain().DisableCollision(this);
	for (auto hitbox : _hitboxes)
	{
		_gc->Contain().Unregister(hitbox);
		_gc->HitPool().Release(hitbox);
	}
}

std::vector<Bullet*>	FireBallMonster::Fire()
{
	std::vector<Bullet*>	bullets;
	return bullets;
}

FireBallMonster*				FireBallMonster::Clone() const
{
	return new FireBallMonster();
}

void					FireBallMonster::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(1);
	}
}

void					FireBallMonster::Update()
{
	if (!_gc)
		return;
	if (_life)
	{
		_pos - Move();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::SPIN);
		this->_gc->Out().push_back(state);
	}
	else
		Die();

}

void						FireBallMonster::Die()
{
	_pos.X() = _gc->Height() + 1;
	_pos.Y() = _gc->Width() + 1;
}

std::vector<FileToSend>		FireBallMonster::Files(const std::string& folder,
													size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "Spinner.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	return list;
}

std::vector<Animation>		FireBallMonster::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 34, 34, 0, 1, 0, AnimationType::BASE, 15));
	list.push_back(Animation(_name, classid, 34, 34, 0, 3, 0, AnimationType::SPIN, 20));
	return list;
}
// ##### FireBallMonster #####

// ##### MalestromBullet #####
BossBullet::BossBullet() : Bullet() {
	_name = "MalestromBullet";
	_life = 100;
}

void			BossBullet::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(2, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	_gc->Contain().Register(this);
	_gc->Contain().EnableCollision(this);
}

void	BossBullet::Alloc(GameObject* src, Vector2D dir, size_t dmg, size_t size2)
{
	_dmg = dmg;
	_pos = src->Position();
	_dir.X() = dir.X();
	_dir.Y() = dir.Y();
	_incx = (float)(_pos.X() - _dir.X());
	_incy = (float)(_pos.Y() - _dir.Y());
	float factor = std::sqrt((_incx * _incx) + (_incy * _incy));
	_decx = _incx / factor;
	_decy = _incy / factor;
	_incx = 0.0;
	_incy = 0.0;
	size_t	size1 = 0;
	for (auto box : src->HitBoxs())
	{
		if (size1 < box->Radius())
			size1 = box->Radius() + 1;
	}
	size_t	size = size1 + size2 + 2;
	while (Vector2D((int)(_pos.X() + _incx), (int)(_pos.Y() + _incy)).distance(_pos) <= (int)size)
	{
		_incx += _decx;
		_incy += _decy;
	}
	_pos.X() -= (int)_incx;
	_pos.Y() -= (int)_incy;
	_incx = _incx - (int)_incx;
	_incy = _incy - (int)_incy;
}

void			BossBullet::Destroy()
{
	if (!this->IsDestroyable())
		return;
	if (!_gc)
		return;
	_gc->Contain().Unregister(this);
	_gc->Contain().DisableCollision(this);
	for (auto hitbox : _hitboxes)
	{
		_gc->Contain().Unregister(hitbox);
		_gc->HitPool().Release(hitbox);
	}
}

BossBullet*	BossBullet::Clone() const
{
	return new BossBullet();
}

std::vector<FileToSend>		BossBullet::Files(const std::string& folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "MoskitoBullet.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	return list;
}

std::vector<Animation>		BossBullet::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	//list.push_back(Animation(_name, classid, 17, 17, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 17, 17, 0, 8, 0, AnimationType::BASE, 10));
	return list;
}
// ##### MalestromBullet #####


// ##### BOSS #####
Boss::Boss() : Monster()
{
	_name = "FinalBoss";
	_scorevalue = 500;
	_ac = _attacks.end();
	_mc = _moves.end();
	_patternMoves.push_back(Vector2D(0, 0));
}

void					Boss::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_life = 65 * _gc->Players();
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(17, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	HitBox*	h2 = _gc->HitPool().Chunk();
	h2->Alloc(17, this, &CollidableObject::Collision);
	h2->Position().Y() = 80;
	this->AddHitBox(h2);
	HitBox*	h3 = _gc->HitPool().Chunk();
	h3->Alloc(17, this, &CollidableObject::Collision);
	h3->Position().Y() = 170;
	this->AddHitBox(h3);
	gc->Contain().Register(this);
	gc->Contain().EnableCollision(this);

	_moves.push_back(MovePattern(MoveDir::UP, 50));
	_moves.push_back(MovePattern(MoveDir::STAY, 100));
	_moves.push_back(MovePattern(MoveDir::UP, 50));
	_moves.push_back(MovePattern(MoveDir::STAY, 50));
	_moves.push_back(MovePattern(MoveDir::UP, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 50));
	_moves.push_back(MovePattern(MoveDir::DOWN, 100));
	_moves.push_back(MovePattern(MoveDir::UP, 200));
	_moves.push_back(MovePattern(MoveDir::STAY, 150));
	// UP 300
	_moves.push_back(MovePattern(MoveDir::DOWN, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 100));
	_moves.push_back(MovePattern(MoveDir::DOWN, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 100));
	_moves.push_back(MovePattern(MoveDir::UP, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 50));
	_moves.push_back(MovePattern(MoveDir::DOWN, 200));
	// 0
	_moves.push_back(MovePattern(MoveDir::UP, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 50));
	_moves.push_back(MovePattern(MoveDir::DOWN, 150));
	_moves.push_back(MovePattern(MoveDir::STAY, 150));
	_moves.push_back(MovePattern(MoveDir::DOWN, 150));
	// DOWN 200
	_moves.push_back(MovePattern(MoveDir::UP, 100));
	_moves.push_back(MovePattern(MoveDir::STAY, 100));
	_moves.push_back(MovePattern(MoveDir::DOWN, 150));
	_moves.push_back(MovePattern(MoveDir::UP, 250));
	// 0

	_attacks.push_back(AttackPatern(AttackType::FIREBALL, 100));
	_attacks.push_back(AttackPatern(AttackType::FIREBALL, 100));
	_attacks.push_back(AttackPatern(AttackType::FIREBALL, 100));
	_attacks.push_back(AttackPatern(AttackType::MOSKITO, 75));
	_attacks.push_back(AttackPatern(AttackType::FIREBALL, 100));
	_attacks.push_back(AttackPatern(AttackType::MOSKITO, 50));
	_attacks.push_back(AttackPatern(AttackType::BULLET, 100));
	_attacks.push_back(AttackPatern(AttackType::BULLET, 75));
	_attacks.push_back(AttackPatern(AttackType::FIREBALL, 100));
	_attacks.push_back(AttackPatern(AttackType::BULLET, 50));
	_attacks.push_back(AttackPatern(AttackType::BULLET, 50));
	_attacks.push_back(AttackPatern(AttackType::MOSKITO, 25));
	_attacks.push_back(AttackPatern(AttackType::MOSKITO, 50));
	_mc = _moves.end();
	_ac = _attacks.end();
	std::random_shuffle(_moves.begin(), _moves.end());
	std::random_shuffle(_attacks.begin(), _attacks.end());
}

void					Boss::Destroy()
{
	if (!this->IsDestroyable())
		return;
	if (!_gc)
		return;
	_gc->Contain().Unregister(this);
	_gc->Contain().DisableCollision(this);
	for (auto hitbox : _hitboxes)
	{
		_gc->Contain().Unregister(hitbox);
		_gc->HitPool().Release(hitbox);
	}
}

std::vector<Bullet*>	Boss::Fire()
{
	std::vector<Bullet*>	bullets;
	return bullets;
}

Boss*				Boss::Clone() const
{
	return new Boss();
}

void					Boss::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(1);
	}
}

void					Boss::Update()
{
	if (!_gc)
		return;
	if (_life)
	{
		_pos - Move();
		Attack();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::BASE);
		this->_gc->Out().push_back(state);
	}
	else
		Die();

}

void						Boss::Attack()
{
	if (_ac != _attacks.end() && (*_ac).current == (*_ac).duration)
	{
		(*_ac).current = 0;
		_ac++;
		if (_ac != _attacks.end())
			(*_ac).current = 0;
	}
	if (_ac == _attacks.end())
	{
		_ac = _attacks.begin();
		(*_ac).current = 0;
	}
	if ((*_ac).type == AttackType::BULLET)
		BulletAttack();
	else if ((*_ac).type == AttackType::MOSKITO)
		MoskitoAttack();
	else
		FireballAttack();
	(*_ac).current += 1;
}

void						Boss::MoskitoAttack()
{
	if ((*_ac).current == 0)
	{
		SceneObject*	elem = _gc->Factory()->Create("Moskito");
		if (elem)
		{
			elem->Position() = _pos;
			elem->Position().X() -= 50;
			elem->Position().Y() += (std::rand() % 174);
			elem->Init(_gc);
		}
	}
}

void						Boss::BulletAttack()
{
	if ((*_ac).current == 0)
	{
		std::vector<BossBullet*>	tmpbullets;
		for (size_t i = 0; i < 16; ++i)
			tmpbullets.push_back(new BossBullet());
		for (auto bullet : tmpbullets)
			bullet->Init(_gc);

		_pos.X() -= 50;
		_pos.Y() += 80;
		tmpbullets[0]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 2, _pos.Y()), 1, 15);
		tmpbullets[1]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 2, _pos.Y() - 1), 1, 15);
		tmpbullets[2]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 2, _pos.Y() - 2), 1, 15);
		tmpbullets[3]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 1, _pos.Y() - 2), 1, 15);
		tmpbullets[4]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X(), _pos.Y() - 2), 1, 15);
		//tmpbullets[5]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 1, _pos.Y() + ydec - 2), 1, 15);
		//tmpbullets[6]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 2, _pos.Y() + ydec - 2), 1, 15);
		//tmpbullets[7]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 2, _pos.Y() + ydec - 1), 1, 15);
		//tmpbullets[8]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 2, _pos.Y() + ydec), 1, 15);
		//tmpbullets[9]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 2, _pos.Y() + ydec + 1), 1, 15);
		//tmpbullets[10]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 2, _pos.Y() + ydec + 2), 1, 15);
		//tmpbullets[11]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() + xdec + 1, _pos.Y() + ydec + 2), 1, 15);
		tmpbullets[12]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X(), _pos.Y() + 2), 1, 15);
		tmpbullets[13]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 1, _pos.Y() + 2), 1, 15);
		tmpbullets[14]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 2, _pos.Y() + 2), 1, 15);
		tmpbullets[15]->Alloc(const_cast<Boss*>(this), Vector2D(_pos.X() - 2, _pos.Y() + 1), 1, 15);
		_pos.X() += 50;
		_pos.Y() -= 80;
	}
}

void						Boss::FireballAttack()
{
	if ((*_ac).current == 0)
	{
		FireBallMonster*	fire = new FireBallMonster();
		fire->Position() = _pos;
		fire->Position().X() -= 50;
		fire->Position().Y() += 80;
		fire->Init(_gc);
	}
}

Vector2D					Boss::Move()
{
	if (_mc != _moves.end() && (*_mc).current == (*_mc).duration)
	{
		(*_mc).current = 0;
		_mc++;
		if (_mc != _moves.end())
			(*_mc).current = 0;
	}
	if (_mc == _moves.end())
	{
		_mc = _moves.begin();
		(*_mc).current = 0;
	}
	(*_mc).current += 1;
	Vector2D	move(0, (((int)((*_mc).dir) - 1) * 3));
	int sum = _pos.Y() - move.Y();
	if (sum >= ((int)_gc->Height() - 174) || sum <= 0)
		move.Y() = 0;
	if (_pos.X() >= (int)(_gc->Width() - 100))
		move.X() = 2;
	return move;
}

void						Boss::Die()
{
	if (_dyingtime > 11)
	{
		_pos.X() = _gc->Height() + 1;
		_pos.Y() = _gc->Width() + 1;
	}
	else
	{
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		GameMessage *state2 = this->_gc->MessagePool()->Chunk();
		GameMessage *state3 = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name), this->Id(), _pos.X(), _pos.Y() + 80, 1, AnimationType::EXPLOSE);
		this->_gc->Out().push_back(state);
		this->_gc->Out().push_back(state2);
		this->_gc->Out().push_back(state3);
	}
	_dyingtime++;
}

std::vector<FileToSend>		Boss::Files(const std::string& folder,
	size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "Boss.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	return list;
}

std::vector<Animation>		Boss::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 76, 174, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 32, 174, 0, 5, 1, AnimationType::EXPLOSE, 10));
	return list;
}
// ##### BOSS #####


std::list<SceneObject*>	PluginFireBall::Package() const
{
	std::list<SceneObject*>	monsters;
	monsters.push_back(new FireBallMonster());
	monsters.push_back(new Boss());
	return monsters;
}

bool					PluginFireBall::CheckDependencies(const std::string& path) const
{
	std::string resources = path.c_str();
	resources += SLASHSEP;
	resources += "Spinner.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "Boss.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "BossBullet.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "explosion.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	return true;
}

extern "C" IPlugin*		loadPlugin()
{
	return new PluginFireBall();
}