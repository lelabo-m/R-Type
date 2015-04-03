#pragma once

#include <functional>
#include <chrono>

class TickTimer
{
private:
	int						_seconds;
	std::function<void(void)>	_funcAssigned;
	std::chrono::time_point<std::chrono::system_clock>	_current;

public:
	TickTimer(int sec, const std::function<void(void)> &);
	~TickTimer();
	void	update();
};

