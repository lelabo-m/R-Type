#pragma once
#include "SFMLshape.h"

class SFMLscore : public SFMLshape
{
private:
	sf::Text	_rank;
	sf::Text	_pseudo;
	sf::Text	_score;
	int			_scoreValue;

public:
	SFMLscore(const std::string &, const std::string &, const std::string &, const sf::Vector2<int> &, int);
	SFMLscore(const SFMLscore &);
	virtual ~SFMLscore();

	virtual void Draw(sf::RenderWindow &);
	int		getScore() const;
};


