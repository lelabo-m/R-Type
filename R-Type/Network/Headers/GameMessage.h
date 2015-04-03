#ifndef _GAMEMESSAGE_H_
# define _GAMEMESSAGE_H_

#include <string>
#include <cstring>
#include "MemoryPool.hpp"

# define GAME_MESSAGE_HEADER_LENGTH 3
# define GAME_MESSAGE_CONTENT_LENGTH 11

class GameMessage : public IChunk<GameMessage>
{
public:
	GameMessage();
	~GameMessage();
	void setGameID(char);
	void setPlayerID(char);
	void setType(char);
	char getGameID() const;
	char getPlayerID() const;
	char getType() const;
	void setSessionID(unsigned int);
	unsigned int getSessionID() const;
	void setCycleShift(int);
	int getCycleShift() const;

	void		Alloc() {}
	void		Free() {}

	template<typename T>
	T *unserializeContent()
	{
		return T::unserialize(_dataOnly);
	}

	template<typename A, typename... Args>
	const std::string &serializeContent(Args... args)
	{
		A::serialize(_dataOnly, args...);

		return _dataOnly;
	}

	std::string serialize()
	{
		char headerOnly[GAME_MESSAGE_HEADER_LENGTH];
		std::memcpy(&headerOnly[0], &_gameID, sizeof(_gameID));
		std::memcpy(&headerOnly[1], &_playerID, sizeof(_playerID));
		std::memcpy(&headerOnly[2], &_type, sizeof(_type));

		_headerOnly.clear();
		_headerOnly.insert(0, headerOnly, GAME_MESSAGE_HEADER_LENGTH);

		return _headerOnly + _dataOnly;
	}

	GameMessage *unseralize(const std::string &wholeData)
	{
		_headerOnly.clear();
		_headerOnly.insert(0, wholeData, 0, GAME_MESSAGE_HEADER_LENGTH);
		const char *content = _headerOnly.c_str();

		_gameID = content[0];
		_playerID = content[1];
		_type = content[2];
		_dataOnly.clear();

		_dataOnly.insert(0, wholeData, GAME_MESSAGE_HEADER_LENGTH, GAME_MESSAGE_CONTENT_LENGTH);
		return this;
	}

protected:
	char _gameID;
	char _playerID;
	char _type;
	int _cycleShift;
	unsigned int _sessionID;
	std::string _headerOnly;
	std::string _dataOnly;
};

class GameMessagePool : public MemoryPool <GameMessage>
{
public:
	GameMessagePool(size_t size) : MemoryPool(size) {}
};

#endif
