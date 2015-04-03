#ifndef MOSKITO_H_
#define MOSKITO_H_

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "Plugin.h"
#include "Monster.h"

class MoskitoBullet : public Bullet
{
public:
	enum AnimationType { BASE, SPIN, EXPLOSE = 42 };
	MoskitoBullet();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual MoskitoBullet*				Clone() const;
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class Moskito : public Monster
{
public:
	enum AnimationType { BASE, FLY, EXPLOSE = 42 };
	Moskito();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual std::vector<Bullet*>		Fire();
	virtual void						Update();
	virtual void						Die();
	virtual Moskito*					Clone() const;
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class MalestromBullet : public Bullet
{
public:
	enum AnimationType { BASE, SPIN, EXPLOSE = 42 };
	MalestromBullet();
	virtual void						Alloc(GameObject*, Vector2D, size_t, size_t);
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual MalestromBullet*			Clone() const;
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class Malestrom : public Monster
{
public:
	enum AnimationType { BASE, FLY, EXPLOSE = 42 };
	Malestrom();
	virtual void						Init(GameContext*);
	virtual	void						Destroy();
	virtual std::vector<Bullet*>		Fire();
	virtual void						Update();
	virtual void						Die();
	virtual Malestrom*					Clone() const;
	virtual void						Collision(ObjectList&);
	virtual std::vector<FileToSend>		Files(const std::string&, size_t) const;
	virtual std::vector<Animation>		Animations(size_t) const;
};

class PluginMoskito : public IPlugin
{
public:
	std::list<SceneObject*>		Package() const;
	bool						CheckDependencies(const std::string&) const;
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
