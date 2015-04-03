#include "GameContainer.h"
#include "ObjectRepresentation.h"

size_t	IdGenerator()
{
	static size_t	id;
	return ++id;
}

// ##### SCENEOBJECT #####
SceneObject::SceneObject(size_t id, size_t x, size_t y, ObjectType type)
	: _id(id), _pos(x, y), _type(type), _name("")
{
	_destroy = false;
}
SceneObject::SceneObject(const SceneObject& other)
	: _gc(other._gc), _id(other._id), _pos(other._pos), _type(other._type), _destroy(other._destroy)
{}
SceneObject::SceneObject(const SceneObject&& other)
	: _gc(std::move(other._gc)), _id(std::move(other._id)), _pos(std::move(other._pos)), _type(std::move(other._type)), _destroy(std::move(other._destroy))
{}
SceneObject&	SceneObject::operator=(const SceneObject& other)
{
	_id = other._id;
	_pos = other._pos;
	_type = other._type;
	_gc = other._gc;
	_destroy = other._destroy;
	return *this;
}
SceneObject&	SceneObject::operator=(const SceneObject&& other)
{
	_id = std::move(other._id);
	_pos = std::move(other._pos);
	_type = std::move(other._type);
	_gc = std::move(other._gc);
	_destroy = std::move(other._destroy);
	return *this;
}
const std::string&	SceneObject::Name() const
{
	return _name;

}
size_t			SceneObject::Id() const
{
	return _id;
}
Vector2D&		SceneObject::Position()
{
	return _pos;
}
ObjectType		SceneObject::Type() const
{
	return _type;
}
bool			SceneObject::OnScene()
{
	if (!_gc)
		return false;
	if (_pos.X() < (int)_gc->Width() && _pos.Y() < (int)_gc->Height()
		&& _pos.X() >= 0 && _pos.Y() >= 0)
		return true;
	return false;
}
void			SceneObject::Debug()
{
	std::cout << "OBJ : ID [" << _id << "] Name [" << _name << "] Type [" << (int)_type
		<< "] POS[" << _pos.X() << " - " << _pos.Y() << "]" << std::endl;
}
bool			SceneObject::IsDestroyable()
{
	if (!_destroy)
	{
		_destroy = true;
		return true;
	}
	return false;
}
// ##### SCENEOBJECT #####

// ##### HITBOX #####
HitBox::HitBox() : SceneObject(IdGenerator(), 0, 0, ObjectType::HITBOX)
{
	_name = "Hitbox";
	_pos.X() = 0;
	_pos.Y() = 0;
}
HitBox::~HitBox() {}

void	HitBox::Init(GameContext *gc)
{
	_gc = gc;
	if (_gc)
		_id = _gc->IdGenerator();
}
void	HitBox::Destroy()
{
	if (!_destroy)
		this->Free();
	if (!this->IsDestroyable())
		return;
	if (!_gc)
		return;
	_gc->HitPool().Release(this);
}


bool	HitBox::HasHit(const HitBox* other) const
{
	if (_parent && other->_parent)
	{
		Vector2D	v;
		v.X() = _parent->Position().X() + _pos.X();
		v.Y() = _parent->Position().Y() + _pos.Y();
		Vector2D	v2;
		v2.X() = other->_parent->Position().X() + other->_pos.X();
		v2.Y() = other->_parent->Position().Y() + other->_pos.Y();
		//std::cout << "ME = " << _parent->Position() << " HITBOX = " << _pos << std::endl;
		//std::cout << "YOU = " << other->_parent->Position() << " HITBOX = " << other->_pos << std::endl;
		//std::cout << "RADIUS = " << v.distance(v2) << " NEED = " << (int)(_radius + other->_radius) << std::endl;
		return ((v.distance(v2) <= (int)(_radius + other->_radius)) ? true : false);
	}
	return false;
}
bool	HitBox::HasHit(CollidableObject*)
{
	if (!_gc)
		return false;
	bool	hit = false;
	if (!_parent)
		return hit;

	for (auto objectlist : _gc->Contain().Content())
	{
		for (auto object : objectlist.second)
		{
			if (_parent != object && _parent->HasCollision(object))
			{
				_hits.push_back(object);
				hit = true;
			}
		}
	}
	return hit;
}
ObjectList	HitBox::Hits() const
{
	if (!_parent)
		return ObjectList();
	return _hits;
}
void		HitBox::Update()
{
	if (!_gc || !_parent)
		return;
	if (HasHit(_parent))
	{
		ObjectList	l = Hits();
		Notify(l);
	}
	_hits.clear();
}
HitBox*		HitBox::Clone() const
{
	return new HitBox();
}
void		HitBox::Alloc(size_t size, CollidableObject* obj, ColliderMethod ptr)
{
	_destroy = false;
	_radius = size;
	_parent = obj;
	_pos.X() = 0;
	_pos.Y() = 0;
	Register(obj, ptr);
}
void		HitBox::Free()
{
	if (!_destroy)
	{
		_parent = nullptr;
		_radius = 0;
		Unregister();
	}
}
std::vector<FileToSend>		HitBox::Files(const std::string&, size_t) const
{
	std::vector<FileToSend> list;
	return list;
}
std::vector<Animation>		HitBox::Animations(size_t) const
{
	std::vector<Animation> list;
	return list;
}

size_t						HitBox::Radius() const
{
	return _radius;
}
// ##### HITBOX #####

// ##### COLLIDABLEOBJECT #####
CollidableObject::CollidableObject(ObjectType type) : SceneObject(IdGenerator(), 0, 0, type) {}

HitBoxList&		CollidableObject::HitBoxs()
{
	return _hitboxes;
}
bool			CollidableObject::HasCollision(CollidableObject* other)
{
	for (auto myhitbox : _hitboxes)
	{
		for (auto otherhitbox : other->_hitboxes)
			if (myhitbox->HasHit(otherhitbox))
				return true;
	}
	return false;
}
void			CollidableObject::AddHitBox(HitBox* h)
{
	if (!_gc)
		return;
	_gc->Contain().Register(h);
	h->Init(_gc);
	_hitboxes.push_back(h);
}
// ##### COLLIDABLEOBJECT #####

// ##### GAMEBJECT #####
GameObject::GameObject(ObjectType type) : CollidableObject(type), _dyingtime(0), _life(1), _velocity(1), _animation(0) 
{}

void		GameObject::Damage(size_t dmg)
{
	if (_life > dmg)
		_life -= dmg;
	else
		_life = 0;
}

size_t		GameObject::Life() const
{
	return _life;
}
bool		GameObject::IsDead() const
{
	return (_life == 0 ? true : false);
}
void		GameObject::AddHitBox(HitBox* h)
{
	if (!_gc)
		return;
	_gc->Contain().Register(h);
	h->Init(_gc);
	_hitboxes.push_back(h);
}
// ##### GAMEBJECT #####

// ##### BULLET #####
Bullet::Bullet() : GameObject(ObjectType::OTHER), _decx(0.0), _decy(0.0), _incx(0.0), _incy(0.0)
{}

void	Bullet::Alloc(GameObject* src, GameObject* target, size_t dmg)
{
	_dmg = dmg;
	_pos = src->Position();
	_dir = target->Position();
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

void	Bullet::Free()
{
	_dmg = 0;
	_pos = Vector2D(0, 0);
	_dir = Vector2D(0, 0);
	_decx = 0.0;
	_decy = 0.0;
	_incx = 0.0;
	_incy = 0.0;
}

void		Bullet::Collision(ObjectList& hits)
{
	for (auto object : hits)
	{
		GameObject*	obj = dynamic_cast<GameObject*>(object);
		if (obj)
			obj->Damage(_dmg);
	}
}

void		Bullet::Update()
{
	if (_life)
	{
		_pos = _pos - Move();
		GameMessage *state = this->_gc->MessagePool()->Chunk();
		//short classID, short objectID, short posX, short posY, char velocity, short animation
		state->serializeContent<ObjectRepresentation>(_gc->Factory()->ClassId(_name),
			this->Id(), _pos.X(), _pos.Y(), 1, 0);
		this->_gc->Out().push_back(state);
	}
}

Vector2D	Bullet::Move()
{
	_incx += _decx * 3;
	_incy += _decy * 3;
	Vector2D inc((int)_incx, (int)_incy);
	_incx = _incx - (int)_incx;
	_incy = _incy - (int)_incy;
	return inc;
}
void		Bullet::Die()
{
	_life = 0;
	_pos.X() = _gc->Height() + 1;
	_pos.Y() = _gc->Width() + 1;
}

// ##### BULLET #####
