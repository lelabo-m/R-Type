#ifndef TIMER_H_
# define TIMER_H_

#include <string>
#include <chrono>
#include <iostream>

struct Time
{
public:
	size_t Minutes;
	size_t Seconds;
	size_t Miliseconds;

public:
	bool	operator>(const Time &) const;
	bool	operator>=(const Time &) const;
	bool	operator<(const Time &) const;
	bool	operator<=(const Time &) const;
	bool	operator==(const Time &) const;
	bool	operator!=(const Time &) const;
	const Time	&operator/=(int value);
	const Time	&operator*=(int value);
};

class Timer
{
public:
	Timer();
	~Timer();
	void Start();
	void Pause();
	void Stop();
	const std::string GetTimerString();
	const Time	&GetTimer() const;
	void SetTimer(size_t, size_t, size_t);
private:
	void ResetTimer();
	void Refresh();
	void AddToTimer(size_t);

private:
	std::chrono::time_point<std::chrono::system_clock>	_saved;
	bool	_running;
	Time	_Timer;
};

#endif // !TIMER_H_
