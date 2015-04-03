#include <iostream>
#include "SFMLobjRepresentation.h"
#include "ClientManager.h"

SFMLobjRepresentation::SFMLobjRepresentation()
{
}

SFMLobjRepresentation::SFMLobjRepresentation(SFMLanimation *anim, ObjectRepresentation *obj) : SFMLshape(*anim->_background.getTexture(), sf::Vector2<int>(obj->getPosX() - anim->getSizeSprite().x / 2, obj->getPosY() - anim->getSizeSprite().y / 2))
{	

	//this->_background.setTextureRect(sf::Rect<int>(obj->getAnimation() * anim->getSizeSprite().x, anim->Index() * anim->getSizeSprite().y,
	//												anim->getSizeSprite().x, anim->getSizeSprite().y));
}

SFMLobjRepresentation::~SFMLobjRepresentation()
{
}

void	SFMLobjRepresentation::Draw(sf::RenderWindow &win)
{

}