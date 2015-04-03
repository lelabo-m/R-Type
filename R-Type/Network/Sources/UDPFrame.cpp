#include <cstring>
#include "UDPFrame.h"

UDPFramePool::UDPFramePool()
{
	for (unsigned int i = 0; i < UDPFRAME_POOL_BASE_LENGTH; i++)
	{
		_frames.push_back(new UDPFrame(i, this));
		_isUsed.push_back(false);
	}
}

UDPFramePool::~UDPFramePool()
{
	for (auto it = _frames.begin(); it != _frames.end(); it++)
	{
		delete *it;
	}
}

void UDPFramePool::release(unsigned int frameID)
{
	_poolMutex.lock();
	if (_isUsed.size() > frameID)
		_isUsed[frameID] = false;
	_poolMutex.unlock();
}

UDPFrame *UDPFramePool::createUDPFrame()
{
	unsigned current_size = _isUsed.size();
	unsigned int i;
	for (i = 0; i < current_size; i++)
	{
		if (_isUsed[i] == false)
		{
			_poolMutex.lock();
			_isUsed[i] = true;
			_poolMutex.unlock();
			return _frames[i];
		}
	}
	_poolMutex.lock();
	_isUsed.push_back(true);
	_isUsed[i] = true;
	_frames.push_back(new UDPFrame(i, this));
	_poolMutex.unlock();
	return _frames[i];
}

UDPFrame::UDPFrame()
{
	_poolId = 0;
	_pool = NULL;
	_data = new char[UDP_HEADER_LENGTH + UDP_FRAME_CONTENT_LENGTH];
	std::memset(_data, 0, UDP_HEADER_LENGTH + UDP_FRAME_CONTENT_LENGTH);
}

UDPFrame::UDPFrame(unsigned int poolId, UDPFramePool *pool)
{
	_poolId = poolId;
	_pool = pool;
	_data = new char[UDP_HEADER_LENGTH + UDP_FRAME_CONTENT_LENGTH];
}

UDPFrame::~UDPFrame()
{
	delete []_data;
}

UDPFrame &UDPFrame::operator=(const UDPFrame &other)
{
	other.getHeader().setDataFromHeader(_data);
	this->_header.setFromData(_data);
	std::memcpy(this->_data, other._data, UDP_FRAME_CONTENT_LENGTH);
	return (*this);
}

char *UDPFrame::getData()
{
	return _data;
}

UDPHeader &UDPFrame::getHeader()
{
	return _header;
}

const UDPHeader &UDPFrame::getHeader() const
{
	return _header;
}

void UDPFrame::release()
{
	if (_pool)
	_pool->release(_poolId);
}
