#pragma once
#include "SFMLshape.h"
#include "SFMLanimatedSprite.h"
#include "ObjectRepresentation.h"

class SFMLobjRepresentation :
	public SFMLshape
{
protected:
	SFMLobjRepresentation();

public:
	SFMLobjRepresentation(SFMLanimation *, ObjectRepresentation *);
	virtual ~SFMLobjRepresentation();
	virtual void Draw(sf::RenderWindow &);
};