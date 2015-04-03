#include "GameSession.h"


GameSession::GameSession(const std::string &gamename, const std::string &mapname, unsigned int pAmount) :
GameName(gamename),
MapName(mapname),
PlayerAmount(pAmount)
{
}

GameSession::~GameSession()
{
}
