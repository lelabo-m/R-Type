#ifndef _UDPFRAME_H_
# define _UDPFRAME_H_

#include <mutex>
#include <vector>
#include "UDPHeader.h"
#include "GameMessage.h"

# define NB_MESSAGE_PER_PACKET 35
# define UDP_FRAME_CONTENT_LENGTH ((GAME_MESSAGE_HEADER_LENGTH + GAME_MESSAGE_CONTENT_LENGTH) * NB_MESSAGE_PER_PACKET)
# define UDPFRAME_POOL_BASE_LENGTH 64

class UDPFramePool;

class UDPFrame
{
public:
	UDPFrame();
	UDPFrame(unsigned int poolId, UDPFramePool *pool);
	~UDPFrame();

	UDPFrame &operator=(const UDPFrame &);
	char *getData();
	UDPHeader &getHeader();
	const UDPHeader &getHeader() const;
	void release();

protected:
	UDPFramePool *_pool;
	unsigned int _poolId;
	UDPHeader _header;
	char *_data;
	bool _used;
};

class UDPFramePool
{
public:
	UDPFramePool();
	~UDPFramePool();
	void release(unsigned int frameID);
	UDPFrame *createUDPFrame();
protected:
	std::mutex _poolMutex;
	std::vector<UDPFrame*> _frames;
	std::vector<bool> _isUsed;
};



#endif