#include <algorithm>
#include "PlayerShip.hh"
#include "GameContainer.h"
#include "ObjectRepresentation.h"
#include "PlayerAction.h"

// ##### PLAYERBULLET #####
PlayerBullet::PlayerBullet()
{
	_name = "PlayerBullet";
	_life = 100;
	_shooter = nullptr;
}
PlayerBullet*	PlayerBullet::Clone() const
{
	return new PlayerBullet();
}

void				PlayerBullet::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(6, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	_gc->Contain().Register(this);
	_gc->Contain().EnableCollision(this);
}

void				PlayerBullet::Destroy(void)
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

void		PlayerBullet::Collision(ObjectList& hits)
{
	if (!_life)
		return;
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
		{
			if (obj->Name() != Name())
				Die();
			obj->Damage(_dmg);
			if (_shooter)
				Notify(obj);
		}
	}
}

void				PlayerBullet::Alloc(PlayerShip* player, size_t dmg)
{
	_shooter = player;
	Register(player, &PlayerShip::MonsterExplode);
	_dmg = dmg;
	_pos = player->Position();
	size_t	size1 = 0;
	for (auto box : player->HitBoxs())
	{
		if (size1 < box->Radius())
			size1 = box->Radius() + 1;
	}
	size_t	size2 = 0;
	for (auto box : this->HitBoxs())
	{
		if (size2 < box->Radius())
			size2 = box->Radius() + 1;
	}
	size_t	size = size1 + size2;
	_pos.X() += size;
}

void				PlayerBullet::Free()
{
	Unregister();
	_shooter = nullptr;
}

Vector2D				PlayerBullet::Move()
{
	Vector2D inc(-4, 0);
	return inc;
}

std::vector<FileToSend>		PlayerBullet::Files(const std::string &folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "PlayerBullet.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	return list;
}

std::vector<Animation>		PlayerBullet::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	//list.push_back(Animation(_name, classid, 33, 12, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 33, 12, 0, 2, 0, AnimationType::BASE, 10));
	return list;
}
// ##### PLAYERBULLET #####

// ##### PlayerBombe #####
PlayerBombe::PlayerBombe() : _targetpos(0, 0)
{
	_name = "PlayerBombe";
	_life = 1000000000;
	_dmg = 30;
	_shooter = nullptr;
}
PlayerBombe*	PlayerBombe::Clone() const
{
	return new PlayerBombe();
}

void				PlayerBombe::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(6, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	_gc->Contain().Register(this);
	_gc->Contain().EnableCollision(this);
}

void				PlayerBombe::Destroy(void)
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

void		PlayerBombe::Damage(size_t)
{
}

void		PlayerBombe::Collision(ObjectList& hits)
{
	if (!_life)
		return;
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
		{
			if (obj->Type() == ObjectType::MONSTER)
			{
				Die();
				obj->Damage(_dmg);
				if (_shooter)
					Notify(obj);
			}
		}
	}
}

void				PlayerBombe::Alloc(PlayerShip* player, GameObject* target, size_t dmg)
{
	_shooter = player;
	Register(player, &PlayerShip::MonsterExplode);
	_dmg = dmg;
	_target = target;
	_pos = player->Position();
	_dir = target->Position();
	_incx = (float)(_pos.X() - _dir.X());
	_incy = (float)(_pos.Y() - _dir.Y());
	float factor = std::sqrt((_incx * _incx) + (_incy * _incy));
	_decx = _incx / factor;
	_decy = _incy / factor;
	_incx = 0.0;
	_incy = 0.0;
	size_t	size1 = 0;
	for (auto box : player->HitBoxs())
	{
		if (size1 < box->Radius())
			size1 = box->Radius() + 1;
	}
	size_t	size2 = 0;
	for (auto box : this->HitBoxs())
	{
		if (size2 < box->Radius())
			size2 = box->Radius() + 1;
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

void				PlayerBombe::Free()
{
	Unregister();
	_shooter = nullptr;
}

Vector2D				PlayerBombe::Move()
{
	if (_gc->Contain().Content().count(ObjectType::MONSTER))
	{
		auto l = _gc->Contain().Content()[ObjectType::MONSTER];
		auto found = std::find_if(l.begin(), l.end(), [this](const GameObject* test) { return (test == this->_target); });
		if (found != l.end())
			_dir = _target->Position();
	}
	_incx = (float)(_pos.X() - _dir.X());
	_incy = (float)(_pos.Y() - _dir.Y());
	float factor = std::sqrt((_incx * _incx) + (_incy * _incy));
	if (factor != 0.0f)
	{
		_decx = _incx / factor;
		_decy = _incy / factor;
		_incx = 0.0;
		_incy = 0.0;
	}
	_incx += _decx * 4;
	_incy += _decy * 4;
	Vector2D inc((int)_incx, (int)_incy);
	_incx = _incx - (int)_incx;
	_incy = _incy - (int)_incy;
	return inc;
}

std::vector<FileToSend>		PlayerBombe::Files(const std::string &folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "PlayerBombe.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	return list;
}

std::vector<Animation>		PlayerBombe::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	//list.push_back(Animation(_name, classid, 33, 12, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 16, 16, 0, 6, 0, AnimationType::BASE, 5));
	return list;
}
// ##### PLAYERBULLET #####

// ##### PLAYERSHIP #####
PlayerShip::PlayerShip() : GameObject(ObjectType::PLAYER), _oldfire(0, 0), _oldbombe(0, 0)
{
	_name = PLAYERSHIP;
	_life = 1;
	_totallife = 3;
	_bombe = 3;
	_oldfire.fired = true;
	_oldbombe.fired = true;
	_actions[Gpa::Action::LEFT] = false;
	_actions[Gpa::Action::RIGHT] = false;
	_actions[Gpa::Action::UP] = false;
	_actions[Gpa::Action::DOWN] = false;
	_actions[Gpa::Action::FIRE] = false;
	_actions[Gpa::Action::BONUS] = false;
	_actionsmethod[Gpa::Action::LEFT] = &PlayerShip::Left;
	_actionsmethod[Gpa::Action::RIGHT] = &PlayerShip::Right;
	_actionsmethod[Gpa::Action::UP] = &PlayerShip::Up;
	_actionsmethod[Gpa::Action::DOWN] = &PlayerShip::Down;
	_actionsmethod[Gpa::Action::FIRE] = &PlayerShip::ShotFire;
	_actionsmethod[Gpa::Action::BONUS] = &PlayerShip::ShotBombe;
}

void	PlayerShip::Alloc(IPlayer* p)
{
	_player = p;
}
void	PlayerShip::Free()
{
	_player = nullptr;
}
PlayerShip*		PlayerShip::Clone() const
{
	return new PlayerShip();
}
PlayerBullet*	PlayerShip::Fire()
{
	return new PlayerBullet();
}
void			PlayerShip::MonsterExplode(GameObject* obj)
{
	if (obj->IsDead())
	{
		Monster*	monster = dynamic_cast<Monster*>(obj);
		if (_player && monster)
			_player->UpdateScores(monster->ScoreValue());
	}
	else if (_player)
		_player->UpdateScores(1);
}
void			PlayerShip::Update()
{
	if (_life && _player)
	{
		for (auto it = _actions.begin(); it != _actions.end(); ++it)
			it->second = false;
		// GET THE ACTIONS FROM CLIENT
		for (auto it = _gc->In().begin(); it != _gc->In().end();)
		{
			auto msg = *it;
			if (msg->getPlayerID() == (char)_player->GetId())
			{
				PlayerAction*	action = msg->unserializeContent<PlayerAction>();
				if (action->getAction() == Gpa::Action::FIRE)
					_fireballs.push_back(FireBall(msg->getCycleShift(), _gc->Time()));
				if (action->getAction() == Gpa::Action::BONUS)
					_bombes.push_back(FireBall(msg->getCycleShift(), _gc->Time()));
				_actions[(int)action->getAction()] = true;
				delete action;
				delete msg;
				it = _gc->In().erase(it);
			}
			else
				++it;
		}
		Vector2D	old = _pos;
		for (auto it = _actions.begin(); it != _actions.end(); ++it)
		{
			if (it->second)
			{
				PlayerShipMethod func = _actionsmethod[it->first];
				(this->*func)();
			}
		}
		AnimationType	anim = ((old.Y() > _pos.X()) ? (AnimationType::DOWN) : (old.Y() < _pos.X())
			? (AnimationType::UP) : (AnimationType::BASE));
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name), this->Id(), _pos.X(), _pos.Y(), 1, ENUMVAL(_player->GetId() % 5, anim));
		this->_gc->Out().push_back(state);
	}
	else
		Die();
}
void			PlayerShip::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(1);
	}
}
void			PlayerShip::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(8, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	gc->Contain().Register(this);
	gc->Contain().EnableCollision(this);
}
void			PlayerShip::Destroy(void)
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
std::vector<FileToSend>	PlayerShip::Files(const std::string &folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "PlayerShip.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "PlayerShip2.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	local = folder + std::string(SLASHSEP) + "PlayerShip3.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_2"));
	local = folder + std::string(SLASHSEP) + "PlayerShip4.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_3"));
	local = folder + std::string(SLASHSEP) + "PlayerShip5.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_4"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_5"));
	return list;
}
std::vector<Animation>	PlayerShip::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 33, 15, 0, 1, 0, ENUMVAL(0, AnimationType::BASE), 1));
	list.push_back(Animation(_name, classid, 33, 15, 0, 3, 0, ENUMVAL(0, AnimationType::UP), 20));
	list.push_back(Animation(_name, classid, 33, 15, 1, 3, 0, ENUMVAL(0, AnimationType::DOWN), 20));

	list.push_back(Animation(_name, classid, 33, 15, 0, 1, 1, ENUMVAL(1, AnimationType::BASE), 1));
	list.push_back(Animation(_name, classid, 33, 15, 0, 3, 1, ENUMVAL(1, AnimationType::UP), 20));
	list.push_back(Animation(_name, classid, 33, 15, 1, 3, 1, ENUMVAL(1, AnimationType::DOWN), 20));

	list.push_back(Animation(_name, classid, 33, 15, 0, 1, 2, ENUMVAL(2, AnimationType::BASE), 1));
	list.push_back(Animation(_name, classid, 33, 15, 0, 3, 2, ENUMVAL(2, AnimationType::UP), 20));
	list.push_back(Animation(_name, classid, 33, 15, 1, 3, 2, ENUMVAL(2, AnimationType::DOWN), 20));

	list.push_back(Animation(_name, classid, 33, 15, 0, 1, 3, ENUMVAL(3, AnimationType::BASE), 1));
	list.push_back(Animation(_name, classid, 33, 15, 0, 3, 3, ENUMVAL(3, AnimationType::UP), 20));
	list.push_back(Animation(_name, classid, 33, 15, 1, 3, 3, ENUMVAL(3, AnimationType::DOWN), 20));

	list.push_back(Animation(_name, classid, 33, 15, 0, 1, 4, ENUMVAL(4, AnimationType::BASE), 1));
	list.push_back(Animation(_name, classid, 33, 15, 0, 3, 4, ENUMVAL(4, AnimationType::UP), 20));
	list.push_back(Animation(_name, classid, 33, 15, 1, 3, 4, ENUMVAL(4, AnimationType::DOWN), 20));

	list.push_back(Animation(_name, classid, 32, 32, 0, 5, 5, AnimationType::EXPLOSE, 10));
	return list;
}
Vector2D				PlayerShip::Move()
{
	return Vector2D();
}
bool					PlayerShip::IsPlayer(IPlayer* p)
{
	return _player == p;
}
void					PlayerShip::Die()
{
	if (_player)	
		_oldplayer = _player;
	_player = nullptr;
	_life = 0;
	if (_dyingtime > 20 && _totallife > 0)
	{
		_player = _oldplayer;
		_pos.X() = 0;
		_pos.Y() = (_gc->Height() / (_gc->Players() + 1)) * ((_player->GetId() % _gc->Players()) + 1);
		_totallife--;
		_life = 1;
		_dyingtime = 0;
	}
	else if (_dyingtime > 20 && _totallife <= 0)
	{
		_pos.X() = _gc->Height() + 1;
		_pos.Y() = _gc->Width() + 1;
	}
	else
	{
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name), this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::EXPLOSE);
		this->_gc->Out().push_back(state);
	}
	_dyingtime++;
}

void					PlayerShip::NetDie()
{
	_oldplayer = nullptr;
	_player = nullptr;
	_life = 0;
	_dyingtime = 0;
	_totallife = 0;
}

void					PlayerShip::AddBombe()
{
	_bombe += 2;
}

void					PlayerShip::Left()
{
	if (_pos.X() > 3)
		_pos.X() -= 3;
}
void					PlayerShip::Right()
{
	if (_pos.X() < (int)_gc->Width() - 2)
		_pos.X() += 3;
}
void					PlayerShip::Up()
{
	if (_pos.Y() > 3)
		_pos.Y() -= 3;
}
void					PlayerShip::Down()
{
	if (_pos.Y() < (int)_gc->Height() - 2)
		_pos.Y() += 3;
}
void					PlayerShip::ShotFire()
{
	bool toshot = false;
	if (_fireballs.size())
	{
		for (auto it = _fireballs.begin(); it != _fireballs.end(); ++it)
		{
			if ((*it).gamecycle - _oldfire.gamecycle > 75)
			{
				_oldfire.gamecycle = (*it).gamecycle;
				_oldfire.msgcycle = (*it).msgcycle;
				toshot = true;
				break;
			}
		}
		_fireballs.clear();
	}
	if (toshot)
	{
		auto bullet = Fire();
		bullet->Init(_gc);
		bullet->Alloc(this, 5);
	}
}
void					PlayerShip::ShotBombe()
{
	if (_gc->Contain().Content().count(ObjectType::MONSTER) && !_gc->Contain().Content()[ObjectType::MONSTER].size())
		_bombes.clear();
	if (_bombe > 0)
	{
		bool toshot = false;
		if (_bombes.size())
		{
			for (auto it = _bombes.begin(); it != _bombes.end(); ++it)
			{
				if ((*it).gamecycle - _oldbombe.gamecycle > 200)
				{
					_oldbombe.gamecycle = (*it).gamecycle;
					_oldbombe.msgcycle = (*it).msgcycle;
					toshot = true;
					break;
				}
			}
			_bombes.clear();
		}
		if (toshot)
		{
			for (auto it = _gc->Contain().Content()[ObjectType::MONSTER].begin();
				it != _gc->Contain().Content()[ObjectType::MONSTER].end(); ++it)
			{
				PlayerBombe*	bombe = new PlayerBombe();
				bombe->Init(_gc);
				bombe->Alloc(this, *it, 30);
			}
			_bombe--;
		}
	}
}
// ##### PLAYERSHIP #####

std::list<SceneObject*>	PluginPlayerShip::Package() const
{
	std::list<SceneObject*>	monsters;
	monsters.push_back(new PlayerShip());
	monsters.push_back(new PlayerBullet());
	monsters.push_back(new PlayerBombe());
	monsters.push_back(new BombeBonus());
	return monsters;
}

bool					PluginPlayerShip::CheckDependencies(const std::string& path) const
{
	std::string resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerShip.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerShip2.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerShip3.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerShip4.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerShip5.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerBullet.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "PlayerBombe.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "explosion.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	return true;
}

// ##### BombeBonus #####
BombeBonus::BombeBonus() : GameObject()
{
	_name = "BombeBonus";
	_life = 10;
}

void					BombeBonus::Init(GameContext *gc)
{
	_type = ObjectType::OTHER;
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(6, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	gc->Contain().Register(this);
	gc->Contain().EnableCollision(this);
}

void					BombeBonus::Destroy()
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

std::vector<Bullet*>	BombeBonus::Fire()
{
	std::vector<Bullet*>	bullets;
	return bullets;
}

BombeBonus*				BombeBonus::Clone() const
{
	return new BombeBonus();
}

void					BombeBonus::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		PlayerShip*	obj = dynamic_cast<PlayerShip*>(object);
		if (obj)
		{
			obj->AddBombe();
			Die();
		}
	}
}

void					BombeBonus::Update()
{
	if (!_gc)
		return;
	if (_life)
	{
		_pos - Move();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::BASE);
		this->_gc->Out().push_back(state);
	}
	else
		Die();

}

Vector2D					BombeBonus::Move()
{
	return Vector2D(3, 0);
}

void						BombeBonus::Die()
{
	_pos.X() = _gc->Height() + 1;
	_pos.Y() = _gc->Width() + 1;
}

std::vector<FileToSend>		BombeBonus::Files(const std::string& folder,
	size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "PlayerBombe.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	return list;
}

std::vector<Animation>		BombeBonus::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 16, 16, 0, 6, 0, AnimationType::BASE, 5));
	return list;
}
// ##### FireBallMonster #####

extern "C" IPlugin*		loadPlugin()
{
	return new PluginPlayerShip();
}
