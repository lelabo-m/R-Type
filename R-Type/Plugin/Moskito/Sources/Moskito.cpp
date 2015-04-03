#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "GameContainer.h"
#include "Moskito.h"
#include "FileSystem.hpp"
#include "ObjectRepresentation.h"

// ##### MOSKITO #####
Moskito::Moskito() : Monster()
{
	_name = "Moskito";
	_life = 1;
	_scorevalue = 15;
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, -2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
	_patternMoves.push_back(Vector2D(2, 2));
}

void					Moskito::Init(GameContext *gc)
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

void					Moskito::Destroy()
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

std::vector<Bullet*>	Moskito::Fire()
{
	std::vector<Bullet*>	bullets;
	bullets.push_back(new MoskitoBullet());
	return bullets;
}

Moskito*				Moskito::Clone() const
{
	return new Moskito();
}

void					Moskito::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(1);
	}
}

void					Moskito::Update()
{
	if (!_gc)
		return;
	if (_life)
	{
		int shot = std::rand();
		if (shot % 666 == 0)
		{
			auto objectmap = _gc->Contain().Content();
			if (objectmap.find(ObjectType::PLAYER) != objectmap.end() && objectmap[ObjectType::PLAYER].size())
			{
				int player = std::rand();
				player %= objectmap[ObjectType::PLAYER].size();
				GameObject*	target = nullptr;
				size_t	i = 0;
				for (auto it = objectmap[ObjectType::PLAYER].begin();
					it != objectmap[ObjectType::PLAYER].end(); ++it)
				{
					if ((int)i == player)
					{
						target = *it;
						break;
					}
					i++;
				}
				if (target)
				{
					auto list = Fire();
					for (auto bullet : list)
					{
						bullet->Init(_gc);
						bullet->Alloc(this, target, 1);
					}
				}
			}
		}
		_pos - Move();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::FLY);
		this->_gc->Out().push_back(state);
	}
	else
		Die();

}

void						Moskito::Die()
{
	if (_dyingtime > 11)
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

std::vector<FileToSend>		Moskito::Files(const std::string& folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "Moskito.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	return list;
}

std::vector<Animation>		Moskito::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 33, 36, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 33, 36, 0, 8, 0, AnimationType::FLY, 20));
	list.push_back(Animation(_name, classid, 32, 32, 0, 5, 1, AnimationType::EXPLOSE, 10));
	return list;
}
// ##### MOSKITO #####

// ##### MOSKITOBULLET #####
MoskitoBullet::MoskitoBullet() : Bullet() {
	_name = "MoskitoBullet";
	_life = 100;
}

void			MoskitoBullet::Init(GameContext *gc)
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

void			MoskitoBullet::Destroy()
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

MoskitoBullet*	MoskitoBullet::Clone() const
{
	return new MoskitoBullet();
}

std::vector<FileToSend>		MoskitoBullet::Files(const std::string& folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "MoskitoBullet.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	return list;
}

std::vector<Animation>		MoskitoBullet::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	//list.push_back(Animation(_name, classid, 17, 17, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 17, 17, 0, 8, 0, AnimationType::BASE, 10));
	list.push_back(Animation(_name, classid, 32, 32, 0, 5, 1, AnimationType::EXPLOSE, 10));
	return list;
}
// ##### MOSKITOBULLET #####

// ##### Malestrom #####
Malestrom::Malestrom() : Monster()
{
	_name = "Malestrom";
	_life = 1;
	_scorevalue = 20;
	_patternMoves.push_back(Vector2D(2, 0));
}

void					Malestrom::Init(GameContext *gc)
{
	_gc = gc;
	if (!_gc)
		return;
	_id = _gc->IdGenerator();
	HitBox*	h = _gc->HitPool().Chunk();
	h->Alloc(15, this, &CollidableObject::Collision);
	this->AddHitBox(h);
	gc->Contain().Register(this);
	gc->Contain().EnableCollision(this);
}

void					Malestrom::Destroy()
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

std::vector<Bullet*>	Malestrom::Fire()
{
	std::vector<MalestromBullet*>	tmpbullets;
	for (size_t i = 0; i < 16; ++i)
		tmpbullets.push_back(new MalestromBullet());
	for (auto bullet : tmpbullets)
		bullet->Init(_gc);

	tmpbullets[0]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 2, _pos.Y()), 1, 15);
	tmpbullets[1]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 2, _pos.Y() - 1), 1, 15);
	tmpbullets[2]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 2, _pos.Y() - 2), 1, 15);
	tmpbullets[3]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 1, _pos.Y() - 2), 1, 15);
	tmpbullets[4]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X(), _pos.Y() - 2), 1, 15);
	tmpbullets[5]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 1, _pos.Y() - 2), 1, 15);
	tmpbullets[6]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 2, _pos.Y() - 2), 1, 15);
	tmpbullets[7]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 2, _pos.Y() - 1), 1, 15);
	tmpbullets[8]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 2, _pos.Y()), 1, 15);
	tmpbullets[9]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 2, _pos.Y() + 1), 1, 15);
	tmpbullets[10]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 2, _pos.Y() + 2), 1, 15);
	tmpbullets[11]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() + 1, _pos.Y() + 2), 1, 15);
	tmpbullets[12]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X(), _pos.Y() + 2), 1, 15);
	tmpbullets[13]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 1, _pos.Y() + 2), 1, 15);
	tmpbullets[14]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 2, _pos.Y() + 2), 1, 15);
	tmpbullets[15]->Alloc(const_cast<Malestrom*>(this), Vector2D(_pos.X() - 2, _pos.Y() + 1), 1, 15);
	std::vector<Bullet*>	bullets;
	for (auto it = tmpbullets.begin(); it != tmpbullets.end(); ++it)
		bullets.push_back(*it);
	return bullets;
}

Malestrom*				Malestrom::Clone() const
{
	return new Malestrom();
}

void					Malestrom::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(1);
	}
}

void					Malestrom::Update()
{
	if (!_gc)
		return;
	if (_life)
	{
		int shot = std::rand();
		if (shot % 666 == 0)
			Fire();
		_pos - Move();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::FLY);
		this->_gc->Out().push_back(state);
	}
	else
		Die();

}

void						Malestrom::Die()
{
	if (_dyingtime > 20)
	{
		_pos.X() = _gc->Height() + 1;
		_pos.Y() = _gc->Width() + 1;
	}
	else
	{
		if (!_dyingtime)
			Fire();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name), this->Id(), _pos.X(), _pos.Y(), 1, AnimationType::EXPLOSE);
		this->_gc->Out().push_back(state);
	}
	_dyingtime++;
}

std::vector<FileToSend>		Malestrom::Files(const std::string& folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "malestrom.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	return list;
}

std::vector<Animation>		Malestrom::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	list.push_back(Animation(_name, classid, 34, 34, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 34, 34, 0, 14, 0, AnimationType::FLY, 20));
	list.push_back(Animation(_name, classid, 32, 32, 0, 5, 1, AnimationType::EXPLOSE, 10));
	return list;
}
// ##### Malestrom #####

// ##### MalestromBullet #####
MalestromBullet::MalestromBullet() : Bullet() {
	_name = "MalestromBullet";
	_life = 100;
}

void			MalestromBullet::Init(GameContext *gc)
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

void	MalestromBullet::Alloc(GameObject* src, Vector2D dir, size_t dmg, size_t size2)
{
	_dmg = dmg;
	_pos = src->Position();
	_dir = dir;
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

void			MalestromBullet::Destroy()
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

MalestromBullet*	MalestromBullet::Clone() const
{
	return new MalestromBullet();
}

std::vector<FileToSend>		MalestromBullet::Files(const std::string& folder, size_t classid) const
{
	std::vector<FileToSend>	list;
	std::string local = folder + std::string(SLASHSEP) + "MoskitoBullet.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_0"));
	local = folder + std::string(SLASHSEP) + "explosion.png";
	list.push_back(FileToSend(local, std::to_string(classid) + "_1"));
	return list;
}

std::vector<Animation>		MalestromBullet::Animations(size_t classid) const
{
	std::vector<Animation>	list;
	//list.push_back(Animation(_name, classid, 17, 17, 0, 1, 0, AnimationType::BASE, 1));
	list.push_back(Animation(_name, classid, 17, 17, 0, 8, 0, AnimationType::BASE, 10));
	list.push_back(Animation(_name, classid, 32, 32, 0, 5, 1, AnimationType::EXPLOSE, 10));
	return list;
}
// ##### MalestromBullet #####

std::list<SceneObject*>	PluginMoskito::Package() const
{
	std::list<SceneObject*>	monsters;
	monsters.push_back(new Moskito());
	monsters.push_back(new MoskitoBullet());
	monsters.push_back(new Malestrom());
	monsters.push_back(new MalestromBullet());
	return monsters;
}

bool					PluginMoskito::CheckDependencies(const std::string& path) const
{
	std::string resources = path.c_str();
	resources += SLASHSEP;
	resources += "Moskito.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "MoskitoBullet.png";
	if (FileSystem::TypeOfFile(resources) == FileSystem::FileType::INVALID)
		return false;
	resources = path.c_str();
	resources += SLASHSEP;
	resources += "malestrom.png";
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
	return new PluginMoskito();
}