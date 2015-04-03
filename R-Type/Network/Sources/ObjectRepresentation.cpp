#include "ObjectRepresentation.h"

ObjectRepresentation::ObjectRepresentation() :
	_classID(0),
	_objectID(0),
	_posX(0),
	_posY(0),
	_velocity(0),
	_animation(0)
{
}

ObjectRepresentation::ObjectRepresentation(short classID, short objectID, short posX, short posY, char velocity, short animation) :
_classID(classID),
_objectID(objectID),
_posX(posX),
_posY(posY),
_velocity(velocity),
_animation(animation)
{

}

ObjectRepresentation::~ObjectRepresentation()
{
}

short ObjectRepresentation::getClassID() const
{
	return _classID;
}

short ObjectRepresentation::getObjectID() const
{
	return _objectID;
}

short ObjectRepresentation::getPosX() const
{
	return _posX;
}

short ObjectRepresentation::getPosY() const
{
	return _posY;
}

char ObjectRepresentation::getVelocity() const
{
	return _velocity;
}

short ObjectRepresentation::getAnimation() const
{
	return _animation;
}

ObjectRepresentation	&ObjectRepresentation::operator=(const ObjectRepresentation &oth)
{
	this->_animation = oth._animation;
	this->_classID = oth._classID;
	this->_objectID = oth._objectID;
	this->_posX = oth._posX;
	this->_posY = oth._posY;
	this->_velocity = oth._velocity;

	return *this;
}