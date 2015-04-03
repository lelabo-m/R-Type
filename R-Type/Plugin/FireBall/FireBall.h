#ifndef MOSKITO_H_
#define MOSKITO_H_

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "Plugin.h"
#include "Monster.h"

class FireBallMonster : public Monster
{
public:
	enum AnimationType { BASE, SPIN };
	FireBallMonster();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual std::vector<Bullet*>		Fire();
	virtual void						Update();
	virtual void						Die();
	virtual FireBallMonster*			Clone() const;
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class PluginFireBall : public IPlugin
{
public:
	std::list<SceneObject*>		Package() const;
	bool						CheckDependencies(const std::string&) const;
};

class BossBullet : public Bullet
{
public:
	enum AnimationType { BASE, SPIN, EXPLOSE = 42 };
	BossBullet();
	virtual void						Alloc(GameObject*, Vector2D, size_t, size_t);
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual BossBullet*					Clone() const;
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class Boss : public Monster
{

public:
	enum AnimationType { BASE, SPIN, EXPLOSE = 42 };
	enum MoveDir { DOWN = 0, STAY = 1, UP = 2};
	
	struct MovePattern
	{
		MovePattern(MoveDir di, size_t du) : dir(di), duration(du), current(0) {};
		MoveDir		dir;
		size_t		duration;
		size_t		current;
	};

	enum AttackType { FIREBALL, MOSKITO, BULLET };

	struct AttackPatern
	{
		AttackPatern(AttackType t, size_t d) : type(t), duration(d), current(0) {};
		AttackType	type;
		size_t		duration;
		size_t		current;
	};

public:
	Boss();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual std::vector<Bullet*>		Fire();
	virtual void						Update();
	virtual void						Die();
	virtual Boss*						Clone() const;
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
private:
	Vector2D					Move();
	void						Attack();
	void						MoskitoAttack();
	void						BulletAttack();
	void						FireballAttack();
	std::vector<AttackPatern>			_attacks;
	std::vector<AttackPatern>::iterator	_ac;
	std::vector<MovePattern>			_moves;
	std::vector<MovePattern>::iterator	_mc;
};

extern "C"
{
#ifdef _WIN32
	__declspec(dllexport) IPlugin* loadPlugin();
#else
	IPlugin *loadPlugin();
#endif
};

#endif /* !MOSKITO_H_ */
