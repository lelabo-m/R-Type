#ifndef GAMECONTAINER_H_
#define GAMECONTAINER_H_

#include <functional>
#include "Player.hh"
#include "Level.hh"
#include "GameObject.h"
#include "Singleton.hpp"
#include "PluginStorage.hh"
#include "GameMessage.h"
#define HITBOXRESERVE	30
#define BULLETRESERVE	30

class GameContext;

class GameContainer
{
public:
	typedef	std::map<ObjectType, std::list<GameObject*> >	ObjectMap;
	typedef std::list<SceneObject*>							ObjectList;

	explicit GameContainer(GameContext *);
	virtual ~GameContainer();
	void				Register(SceneObject*);
	void				Unregister(SceneObject*);
	void				EnableCollision(GameObject*);
	void				DisableCollision(GameObject*);
	ObjectMap&			Content();
	ObjectList&			Objects();
	void				Update();
private:
	GameContext			*_gc;
	ObjectList			_scene;
	ObjectMap			_container;
};

typedef std::map<std::string, size_t> ObjectTable;
class GameContext
{
public:
	typedef std::list<Player*>	PlayerList;
	typedef std::function<void (GameContext*, PlayerList&)>	InitFunc;
public:
	GameContext(size_t, size_t, ObjectFactory*, GameMessagePool*);
	void	Start(const std::string&, InitFunc,
					std::list<Player*>&);
	size_t						Height() const;
	size_t						Width() const;
	GameContainer&				Contain();
	HitBox::pool&				HitPool();
	ObjectFactory*				Factory();
	GameMessagePool*			MessagePool();
	std::list<GameMessage*>&	In();
	std::list<GameMessage*>&	Out();
	void						Update();
	void						PlayerDie(Player*);
	void						Debug();
	bool						IsFinish();
	size_t						IdGenerator();
	size_t						Time() const;
	size_t						Players() const;
	void						SetPlayers(size_t);
private:
	LevelContent				_mapcontent;
	size_t						_height;
	size_t						_width;
	ObjectFactory*				_factory;
	GameContainer				_container;
	HitBox::pool				_hitboxpool;
	std::list<GameMessage*>		_in;
	std::list<GameMessage*>		_out;
	GameMessagePool*			_messagepool;
	size_t						_time;
	size_t						_id;
	size_t						_nbplayer;
public:
	static InitFunc	RtypeGame();
};

#endif /* !GAMECONTAINER_H_ */
