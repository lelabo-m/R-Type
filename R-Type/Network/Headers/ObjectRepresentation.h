#ifndef _OBJECTREPRESENTATION_H_
# define _OBJECTREPRESENTATION_H_

#include <string>
#include <cstring>
#include "GameMessage.h"

class ObjectRepresentation
{
public:
	static ObjectRepresentation *unserialize(const std::string &data)
	{
		const char *data_str = data.c_str();

		short classID = *((short*)&data_str[0]);
		short objectID = *((short*)&data_str[2]);
		short posX = *((short*)&data_str[4]);
		short posY = *((short*)&data_str[6]);
		char velocity = *((char*)&data_str[8]);
		short animation = *((short*)&data_str[9]);

		return new ObjectRepresentation(classID, objectID, posX, posY, velocity, animation);
	}
	static void serialize(std::string &str, short classID, short objectID, short posX, short posY, char velocity, short animation)
	{
		char _content[GAME_MESSAGE_CONTENT_LENGTH];

		std::memcpy(&_content[0], &classID, sizeof(classID));
		std::memcpy(&_content[2], &objectID, sizeof(objectID));
		std::memcpy(&_content[4], &posX, sizeof(posX));
		std::memcpy(&_content[6], &posY, sizeof(posY));
		std::memcpy(&_content[8], &velocity, sizeof(velocity));
		std::memcpy(&_content[9], &animation, sizeof(animation));

		str.clear();
		str.insert(0, _content, GAME_MESSAGE_CONTENT_LENGTH);
	}
	~ObjectRepresentation();
	ObjectRepresentation();

	short getClassID() const;
	short getObjectID() const;
	short getPosX() const;
	short getPosY() const;
	char getVelocity() const;
	short getAnimation() const;
	ObjectRepresentation	&operator=(const ObjectRepresentation &);

protected:
	ObjectRepresentation(short classID, short objectID, short posX, short posY, char velocity, short animation);

protected:
	short _classID;
	short _objectID;
	short _posX;
	short _posY;
	char _velocity;
	short _animation;
};



#endif
