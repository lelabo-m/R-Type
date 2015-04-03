#include <algorithm>
#include "BackgroundAnimation.h"

BackgroundAnimation::BackgroundAnimation(int min, int max, int start, size_t loopSpeed, animationType t)
{
	this->min	= std::min(min, max);
	this->max	= std::max(min, max);
	this->value = std::min(max, std::max(min, start));
	this->part	= true;
	this->type	= t;
	this->loopSpeed = loopSpeed;
	this->currentLoop = 0;
}

BackgroundAnimation::~BackgroundAnimation()
{
}

void	BackgroundAnimation::next()
{
	if (type == LINEAR)
	{
		if (this->currentLoop == 0)
		{
			if (this->part)
			{
				if (this->value == this->max)
					this->part = false;
				else
					this->value++;
			}
			else
			{
				if (this->value == this->min)
					this->part = true;
				else
					this->value--;
			}
		}
	}
	else
	{
		if (this->currentLoop == 0)
		{
			this->value = (this->value >= this->max ? min : value + 1);
		}
	}
	this->currentLoop++;
	this->currentLoop %= this->loopSpeed;
}

int		BackgroundAnimation::getValue()
{
	return this->value;
}