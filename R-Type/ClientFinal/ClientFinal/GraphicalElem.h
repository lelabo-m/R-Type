#ifndef GRAPHICALELEM_H_
#define GRAPHICALELEM_H_

#include "Vector2D.h"
#include "SFMLanimatedSprite.h"

class GraphicalElem
{
public:
	GraphicalElem(const std::string& name, size_t classid, size_t id, size_t x, size_t y, size_t v);
	const std::string&	Name() const;
	size_t				Id() const;
	size_t				ClassId() const;
	Vector2D&			Position();
	size_t				Velocity() const;
	size_t				Animation() const;
	SFMLanimation*		GetAnimation();
	void				Play(size_t, bool loop = true);
	void				Draw();
private:
	bool			_loop;
	std::string		_name;
	size_t			_classid;
	size_t			_id;
	Vector2D		_pos;
	size_t			_velocity;
	SFMLanimation*	_animation;
};

#endif /* !GRAPHICALELEM_H_*/