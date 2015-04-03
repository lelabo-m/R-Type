#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <vector>
#include <list>
#include <map>
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "Factory.hpp"
#include "MemoryPool.hpp"
#include "Vector2D.h"
#include "Observer.hpp"
#include "ObjectRepresentation.hpp"
#include "FileSystem.hpp"

enum class ObjectType { SCENE, PLAYER, MONSTER, HITBOX, ALL, OTHER };
class SceneObject;
class CollidableObject;
class GameContext;

size_t	IdGenerator();

typedef std::vector<SceneObject*>	SceneObjectList;
typedef	void(SceneObject::*SceneMethod)(SceneObjectList&);
class SceneObject : public IPrototype<SceneObject>
{
public:
	SceneObject(size_t, size_t, size_t, ObjectType);
	SceneObject(const SceneObject&);
	SceneObject(const SceneObject&&);
	SceneObject&	operator=(const SceneObject&);
	SceneObject&	operator=(const SceneObject&&);
	virtual ~SceneObject() {};
	virtual bool		IsDestroyable();
	virtual	void		Init(GameContext *) = 0;
	virtual void		Destroy() = 0;
	virtual void		Update() = 0;
	virtual bool		OnScene();
	virtual size_t		Id() const;
	virtual Vector2D&	Position();
	virtual ObjectType	Type() const;
	virtual const std::string&	Name() const;
	virtual std::vector<FileToSend>		Files(const std::string& path, size_t) const = 0;
	virtual std::vector<Animation>		Animations(size_t) const = 0;
	virtual void						Debug();
protected:
	GameContext*	_gc;
	size_t			_id;
	Vector2D		_pos;
	ObjectType		_type;
	std::string		_name;
	bool			_destroy;
};

typedef std::vector<CollidableObject*>	ObjectList;
typedef	void(CollidableObject::*ColliderMethod)(ObjectList&);

class HitBox : public SceneObject, public IChunk<HitBox, size_t, CollidableObject*, ColliderMethod>,
	public Subject<CollidableObject, ColliderMethod, ObjectList>
{
public:
	typedef MemoryPool<HitBox, size_t, CollidableObject*, ColliderMethod>	pool;
	HitBox();
	~HitBox();
	void				Init(GameContext *);
	void				Destroy();
	bool				HasHit(const HitBox*) const;
	bool				HasHit(CollidableObject*);
	ObjectList			Hits() const;
	size_t				Radius() const;
public:
	void				Update();
	HitBox*				Clone() const;
	void				Alloc(size_t, CollidableObject*, ColliderMethod);
	void				Free();
	virtual std::vector<FileToSend>		Files(const std::string& path, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
private:
	CollidableObject*	_parent;
	size_t				_radius;
	ObjectList			_hits;
};

typedef std::vector<HitBox*>	HitBoxList;
class CollidableObject : public SceneObject,
	public Observer<CollidableObject, ColliderMethod, ObjectList>
{
public:
	CollidableObject(ObjectType);
	virtual ~CollidableObject() {};
	virtual void			Collision(ObjectList&) = 0;
	virtual bool			HasCollision(CollidableObject*);
	virtual HitBoxList&		HitBoxs();
	virtual void			AddHitBox(HitBox*);
protected:
	HitBoxList	_hitboxes;
};

struct PathPoint
{
	Vector2D	pos;
	Vector2D	dir;
	size_t		time;
};

typedef std::vector<PathPoint>	MovesList;
class GameObject : public CollidableObject
{
public:
	GameObject(ObjectType type = ObjectType::MONSTER);
	virtual ~GameObject() {};
	virtual Vector2D		Move() = 0;
	virtual void			Damage(size_t);
	size_t					Life() const;
	bool					IsDead() const;
	virtual void			Die() = 0;
	virtual void			AddHitBox(HitBox*);
protected:
	size_t			_dyingtime;
	size_t			_life;
	size_t			_velocity;
	size_t			_animation;
};

class Bullet : public GameObject, public IChunk<Bullet, GameObject*, GameObject*, size_t>
{
public:
	Bullet();
	void	Alloc(GameObject*, GameObject*, size_t);
	void	Free();
	virtual ~Bullet() {};
	virtual Vector2D	Move();
	virtual void		Collision(ObjectList&);
	virtual void		Update();
	virtual void		Die();
protected:
	Vector2D	_dir;
	size_t		_dmg;
	float		_decx;
	float		_decy;
	float		_incx;
	float		_incy;
};
#endif /* !GAMEOBJECT_H_ */