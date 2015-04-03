#ifndef _GAME_H_
# define _GAME_H_

#include <string>

class Game
{
public:
	enum GameStatus
	{
		WAITING,
		LOADING,
		LOADING_FINISH,
		SYNCHRONIZATION,
		SYNCHRONIZATION_FINISHED,
		GAME_STARTED,
		GAME_INTERUPTION,
		GAME_FINISHED
	};

	Game(const std::string &, const std::string &);
	~Game();
	void setGameStatus(GameStatus);
	GameStatus getGameStatus() const;
	unsigned int getScore() const;
	void setScore(unsigned int score);

protected:
	GameStatus _gameStatus;
	std::string _gameName;
	std::string _mapName;
	unsigned int _score;
};

#endif