#include <iostream>
#include "TickTimer.h"

TickTimer::TickTimer(int sec, const std::function<void(void)> &func)
{
	_current		= std::chrono::system_clock::now();
	_seconds		= sec;
	_funcAssigned	= func;
}

TickTimer::~TickTimer()
{
}

void	TickTimer::update()
{
	std::chrono::time_point<std::chrono::system_clock>	_save = std::chrono::system_clock::now();
	
	auto val = std::chrono::duration_cast<std::chrono::seconds>(_save - _current);
	
	if (val.count() >= _seconds)
	{
		if (_funcAssigned)
			_funcAssigned();
		_current = std::chrono::system_clock::now();
	}
}