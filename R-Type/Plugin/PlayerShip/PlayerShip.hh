#ifndef PLAYERSHIP_H_
#define PLAYERSHIP_H_

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#define PLAYERSHIP "PlayerShip"

#include "Plugin.h"
#include "IPlayer.hh"
#include "GameObject.h"
#include "Monster.h"
#include "GameAction.hh"

typedef int InitMessage;

class PlayerShip;
typedef void(PlayerShip::*PlayerShipMethod)(GameObject*);
class PlayerBullet : public Bullet, public Subject<PlayerShip, PlayerShipMethod, GameObject*>,
	public IChunk<PlayerBullet, PlayerShip*, size_t>
{
public:
	enum AnimationType { BASE, SPIN };
	PlayerBullet();
	virtual PlayerBullet*				Clone() const;
	virtual void 						Init(GameContext *);
	virtual void 						Destroy(void);
	virtual void						Alloc(PlayerShip*, size_t);
	virtual void						Free();
	virtual Vector2D					Move();
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string &, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
private:
	PlayerShip*		_shooter;
};

typedef void(PlayerShip::*PlayerShipMethod)(GameObject*);
class PlayerBombe : public Bullet, public Subject<PlayerShip, PlayerShipMethod, GameObject*>,
	public IChunk<PlayerBullet, PlayerShip*, GameObject*, size_t>
{
public:
	enum AnimationType { BASE, SPIN };
	PlayerBombe();
	void								Damage(size_t);
	virtual PlayerBombe*				Clone() const;
	virtual void 						Init(GameContext *);
	virtual void 						Destroy(void);
	virtual void						Alloc(PlayerShip*, GameObject*, size_t);
	virtual void						Free();
	virtual Vector2D					Move();
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string &, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
private:
	PlayerShip*		_shooter;
	GameObject*		_target;
	Vector2D		_targetpos;
};

class IPlayerShip
{
public:
	virtual	bool			IsPlayer(IPlayer*) = 0;
	virtual PlayerBullet*	Fire() = 0;
	virtual void			MonsterExplode(GameObject*) = 0;
	virtual ~IPlayerShip() {};
};

struct FireBall
{
	FireBall(int m, int g) : msgcycle(m), gamecycle(g), fired(false) {}
	int		msgcycle;
	int		gamecycle;
	bool	fired;
};

#define ENUMVAL(id, enumeration)	((100 * enumeration) + id)

class PlayerShip : public IPlayerShip, public GameObject, public Observer<PlayerShip, PlayerShipMethod, GameObject*>, IChunk<PlayerShip, IPlayer*>
{
	typedef	void(PlayerShip::*PlayerShipMethod)(void);
public:
	enum AnimationType { BASE, UP, DOWN, EXPLOSE = 42 };
	PlayerShip();
	void								Alloc(IPlayer*);
	void								Free();
	virtual void						Init(GameContext *);
	virtual void						Destroy(void);
	virtual std::vector<FileToSend>		Files(const std::string &, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
	virtual void						Update();
	virtual Vector2D					Move();
	virtual void						Die();
	virtual void						NetDie();
	virtual PlayerShip*					Clone() const;
	virtual PlayerBullet*				Fire();
	virtual bool						IsPlayer(IPlayer*);
	virtual void						MonsterExplode(GameObject*);
	virtual void						Collision(ObjectList&);
	void								Up();
	void								Down();
	void								Left();
	void								Right();
	void								ShotFire();
	void								ShotBombe();
	void								AddBombe();
private:
	IPlayer*							_player;
	IPlayer*							_oldplayer;
	std::map<int, bool>					_actions;
	std::map<int, PlayerShipMethod>		_actionsmethod;
	std::list<FireBall>					_fireballs;
	FireBall							_oldfire;
	std::list<FireBall>					_bombes;
	FireBall							_oldbombe;
	size_t								_totallife;
	size_t								_bombe;
};

class PluginPlayerShip : public IPlugin
{
public:
	std::list<SceneObject*>		Package() const;
	bool						CheckDependencies(const std::string&) const;
};

class BombeBonus : public GameObject
{
public:
	enum AnimationType { BASE, SPIN };
	BombeBonus();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual std::vector<Bullet*>		Fire();
	virtual void						Update();
	virtual void						Die();
	virtual Vector2D					Move();
	virtual BombeBonus*					Clone() const;
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

extern "C"
{
#ifdef _WIN32
	__declspec(dllexport) IPlugin* loadPlugin();
#else
	IPlugin *loadPlugin();
#endif
};

#endif /* !PLAYERSHIP_H_ */
