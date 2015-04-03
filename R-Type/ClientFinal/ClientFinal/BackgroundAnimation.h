#pragma once

enum animationType
{
	LINEAR,
	SLIDER
};

class BackgroundAnimation
{
public:
	BackgroundAnimation(int min, int max, int start, size_t, animationType type);
	~BackgroundAnimation();
	void	next();
	int		getValue();

private:
	animationType	type;
	bool			part;
	int				min;
	int				max;
	int				value;
	size_t			loopSpeed;
	size_t			currentLoop;
};

