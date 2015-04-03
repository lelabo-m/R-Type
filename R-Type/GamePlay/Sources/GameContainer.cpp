#include <algorithm>
#include "GameContainer.h"
#include "Server.hh"
#include "PlayerShip.hh"

// ##### GAMECONTEXT #####
GameContext::GameContext(size_t h, size_t w, ObjectFactory *t, GameMessagePool* p)
	: _height(h), _width(w), _factory(t), _container(this),
	_hitboxpool(HITBOXRESERVE), _messagepool(p), _time(0), _id(0), _nbplayer(0)
{
}

size_t	GameContext::IdGenerator()
{
	_id++;
	return _id;
}

size_t	GameContext::Time() const
{
	return _time;
}

size_t	GameContext::Height() const
{
	return _height;
}

size_t	GameContext::Width() const
{
	return _width;
}

GameContainer&	GameContext::Contain()
{
	return _container;
}

HitBox::pool&	GameContext::HitPool()
{
	return _hitboxpool;
}

ObjectFactory	*GameContext::Factory()
{
	return _factory;
}

std::list<GameMessage*>&	GameContext::In()
{
	return _in;
}
std::list<GameMessage*>&	GameContext::Out()
{
	return _out;
}

GameMessagePool*			GameContext::MessagePool()
{
	return _messagepool;
}

size_t						GameContext::Players() const
{
	return _nbplayer;
}

void						GameContext::SetPlayers(size_t pl)
{
	_nbplayer = pl;;
}


void						GameContext::Start(const std::string& map,
												InitFunc func,
												PlayerList& players)
{
	Level			lvl;

	_mapcontent.clear();
	if (lvl.readFromFile(MAPDIR + std::string(SLASHSEP) + FileSystem::FileShortName(map)) == true)
		_mapcontent = lvl.getLevelElems();
	func(this, players);
}

void						GameContext::PlayerDie(Player* p)
{
	for (auto it = _container.Objects().begin(); it != _container.Objects().end(); ++it)
	{
		if ((*it)->Type() == ObjectType::PLAYER)
		{
			PlayerShip*	ship = dynamic_cast<PlayerShip*>(*it);
			if (ship && ship->IsPlayer(p))
			{
				ship->NetDie();
				break;
			}
		}
	}
}

void						GameContext::Debug()
{
	std::cout << "OBJECTLIST : " << std::endl;
	for (auto it = _container.Objects().begin(); it != _container.Objects().end(); ++it)
		(*it)->Debug();
}

void						GameContext::Update()
{
	for (auto it = _out.begin(); it != _out.end();)
	{
		(*it)->Free();
		_messagepool->Release(*it);
		*it = nullptr;
		it = _out.erase(it);
	}
	for (auto it = _mapcontent.begin(); it != _mapcontent.end();)
	{
		if ((*it).time <= _time)
		{
			SceneObject*	elem = _factory->Create((*it).name);
			elem->Position().X() = this->_width;
			elem->Position().Y() = (*it).posY;
			elem->Init(this);
			it = _mapcontent.erase(it);
		}
		else
			++it;
	}
	_container.Update();
	GameContainer::ObjectList	todestroy;
	for (auto it = _container.Objects().begin(); it != _container.Objects().end(); ++it)
	{
		if ((*it)->OnScene() == false)
			todestroy.push_back(*it);
	}
	for (auto it = todestroy.begin(); it != todestroy.end(); ++it)
		(*it)->Destroy();
	while (todestroy.size())
	{
		auto it = todestroy.begin();
		auto obj = *it;
		if (obj->Type() != ObjectType::HITBOX)
			delete obj;
		todestroy.erase(it);
	}
	_time += 2;
	for (auto it = _in.begin(); it != _in.end();)
	{
		delete *it;
		*it = nullptr;
		it = _in.erase(it);
	}
}

bool		GameContext::IsFinish()
{
	if (_container.Content()[ObjectType::PLAYER].size() == 0 || (_container.Content()[ObjectType::MONSTER].size() == 0 && _mapcontent.size() == 0))
		return true;
	return false;
}

// GAMETYPE
GameContext::InitFunc	GameContext::RtypeGame()
{
	InitFunc	rtype_function = ([](GameContext* context, PlayerList& players) {
		size_t i = 1;
		context->SetPlayers(players.size());
		for (auto player : players)
		{
			// Do something
			SceneObject*	p = context->Factory()->Create(PLAYERSHIP);
			PlayerShip*		ship = dynamic_cast<PlayerShip*>(p);
			if (ship)
			{
				ship->Position().Y() = (context->Height() / (players.size() + 1)) * i;
				ship->Alloc(player);
				ship->Init(context);
				i++;
			}
		}
	});
	return (rtype_function);
}

// ##### GAMECONTEXT #####

// ##### GAMECONTAINER #####
GameContainer::GameContainer(GameContext *gc) : _gc(gc)
{
}

GameContainer::~GameContainer()
{
	_scene.clear();
	_container.clear();
}

void	GameContainer::Register(SceneObject* obj)
{
	_scene.push_back(obj);
}

void	GameContainer::Unregister(SceneObject* obj)
{
	for (auto it = _scene.begin(); it != _scene.end(); ++it)
		if (*it == obj)
		{
			*it = nullptr;
			_scene.erase(it);
			break;
		}
}

void	GameContainer::EnableCollision(GameObject* obj)
{
	_container[obj->Type()].push_back(obj);
}

void	GameContainer::DisableCollision(GameObject* obj)
{
	if (_container.count(obj->Type()))
	{
		auto it = std::find(std::begin(_container[obj->Type()]),
			std::end(_container[obj->Type()]), obj);
		if (it != _container[obj->Type()].end())
		{
			*it = nullptr;
			_container[obj->Type()].erase(it);
		}
	}
}

GameContainer::ObjectMap&	GameContainer::Content()
{
	return _container;
}

GameContainer::ObjectList&	GameContainer::Objects()
{
	return _scene;
}

void	GameContainer::Update()
{
	for (auto it = _scene.begin(); it != _scene.end(); it++)
		(*it)->Update();
}

// ##### GAMECONTAINER #####

