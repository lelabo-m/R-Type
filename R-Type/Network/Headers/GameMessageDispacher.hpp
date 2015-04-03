#ifndef _GAMEMESSAGEDISPACHER_HPP_
# define _GAMEMESSAGEDISPACHER_HPP_

#include <list>
#include "GameMessage.h"
#include "UDPFrame.h"

namespace GameMessageDispacher
{
	static UDPFrame *fillFrame(UDPFrame *frame, std::list<GameMessage *> &list, GameMessagePool *pool = nullptr)
	{
		char *data = frame->getData();
		data = &data[UDP_HEADER_LENGTH];

		auto it = list.begin();
		unsigned int a = 0;
		for (unsigned int i = 0; i < NB_MESSAGE_PER_PACKET; i++)
		{
			std::string debug = (*it)->serialize();
		   std::memcpy(&data[a], (*it)->serialize().c_str(), GAME_MESSAGE_HEADER_LENGTH + GAME_MESSAGE_CONTENT_LENGTH);
			a += GAME_MESSAGE_HEADER_LENGTH + GAME_MESSAGE_CONTENT_LENGTH;

			if (pool)
				pool->Release(*it);
			else
				delete (*it);
			it = list.erase(it);
			if (it == list.end())
			{
				frame->getHeader().setNOPE(static_cast<char>(i + 1));
				return frame;
			}
		}
		frame->getHeader().setNOPE(static_cast<char>(NB_MESSAGE_PER_PACKET));
		return frame;
	}

	static void extractFrame(UDPFrame *frame, std::list<GameMessage *> &list, unsigned int playerID = 0, int cycleShift = 0)
	{
		char *data = &frame->getData()[UDP_HEADER_LENGTH];

		int a = 0;
		for (int i = 0; i < frame->getHeader().getNOPE(); i++)
		{
			std::string str;
			list.push_back(new GameMessage());
			str.insert(0, &data[a], GAME_MESSAGE_HEADER_LENGTH + GAME_MESSAGE_CONTENT_LENGTH);
			list.back()->unseralize(str);
			list.back()->setPlayerID(static_cast<char>(playerID));
			list.back()->setSessionID(playerID);
			list.back()->setCycleShift(cycleShift);

			a += GAME_MESSAGE_HEADER_LENGTH + GAME_MESSAGE_CONTENT_LENGTH;
		}
	}
}

#endif
