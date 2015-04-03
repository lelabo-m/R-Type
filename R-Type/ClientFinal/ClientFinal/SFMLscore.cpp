#include <sstream>
#include "SFMLscore.h"
#include "SFMLbasicText.h"

SFMLscore::SFMLscore(const std::string &rank, const std::string &pseudo, const std::string &score, const sf::Vector2<int> &pos, int charSize)
{
	_rank	= SFMLbasicText(rank, pos, charSize);
	_pseudo = SFMLbasicText(pseudo, sf::Vector2<int>(pos.x + 100, pos.y), charSize);
	_score	= SFMLbasicText(score, sf::Vector2<int>(pos.x + 530, pos.y), charSize);
	std::istringstream(score) >> _scoreValue;
}

SFMLscore::SFMLscore(const SFMLscore &s) : SFMLshape()
{
    this->_rank = s._rank;
    this->_pseudo = s._pseudo;
    this->_score = s._score;
}

SFMLscore::~SFMLscore() {}

void	SFMLscore::Draw(sf::RenderWindow &window)
{
	window.draw(_rank);
	window.draw(_pseudo);
	window.draw(_score);
}

int		SFMLscore::getScore() const
{
	return this->_scoreValue;
}