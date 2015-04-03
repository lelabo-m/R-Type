#pragma once

#include "SFMLdropdown.h"

enum dropdownType
{
	FIRSTS,
	LASTS
};

class SFMLunrotateDropdown : public SFMLdropdown
{
protected:
	int				_currentStage;
	dropdownType	_type;

public:
	SFMLunrotateDropdown(const sf::Texture &, const sf::Vector2<int> &, const sf::Texture &, const sf::Vector2<int> &, const sf::Vector2<int> &, int nbVisible = 0, dropdownType = FIRSTS);
	~SFMLunrotateDropdown();

private:
	void	upButton(SFMLshape *);
	void	downButton(SFMLshape *);
	void	initDropdown();

public:
	virtual void Draw(sf::RenderWindow &);
	virtual void Clear();
	virtual void Add(SFMLshape *);
};