#include "Game.h"


Game::Game(const std::string &gameName, const std::string &mapName) : _gameStatus(WAITING), _gameName(gameName), _mapName(mapName)
{
}

Game::~Game()
{
}

void Game::setGameStatus(Game::GameStatus gs)
{
	_gameStatus = gs;
}

Game::GameStatus Game::getGameStatus() const
{
	return _gameStatus;
}

unsigned int Game::getScore() const
{
	return _score;
}

void Game::setScore(unsigned int score)
{
	_score = score;
}