#include "GraphicalElem.h"
#include "ClientManager.h"
#include "windowSFML.h"

GraphicalElem::GraphicalElem(const std::string& name, size_t classid, size_t id, size_t x, size_t y, size_t v)
	: _loop(true), _name(name.c_str()), _classid(classid), _id(id), _pos(x, y), _velocity(v), _animation(nullptr)
{}

Vector2D&	GraphicalElem::Position()
{
	return _pos;
}

const std::string&	GraphicalElem::Name() const
{
	return _name;
}

size_t		GraphicalElem::ClassId() const
{
	return _classid;
}

size_t		GraphicalElem::Id() const
{
	return _id;
}

size_t		GraphicalElem::Animation() const
{
	if (_animation)
		return _animation->Current();
	return 0;
}

SFMLanimation*	GraphicalElem::GetAnimation()
{
	ClientManager*	client = ClientManager::getInstance();
	if (!_animation)
		_animation = client->AnimManager().GetAnimation(_classid, 0);
	return _animation;
}

void			GraphicalElem::Play(size_t anim, bool loop)
{
	_loop = loop;
	ClientManager*	client = ClientManager::getInstance();
	if (client != nullptr)
	{
		_animation->setLoop(loop);
		_animation = client->AnimManager().GetAnimation(_classid, anim);
	}
}

void			GraphicalElem::Draw()
{
	ClientManager*	client = ClientManager::getInstance();
	if (!_animation && client)
		_animation = client->AnimManager().GetAnimation(_classid, 0);
	if (_animation && windowSFML::getWindow())
	{
		if (_loop)
			_animation->PlayLoop(_pos.X(), _pos.Y(), *windowSFML::getWindow());
		else
			_animation->Play(_pos.X(), _pos.Y(), *windowSFML::getWindow());
	}
}