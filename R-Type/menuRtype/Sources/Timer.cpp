#include <sstream>
#include <algorithm>
#include "Timer.h"

Timer::Timer()
{
	_running = false;
	_saved = std::chrono::system_clock::now();
	SetTimer(0, 0, 0);
}

Timer::~Timer()
{

}

void Timer::Start()
{
	this->_running = true;
}

void Timer::Stop()
{
	this->Pause();
	ResetTimer();
}

void	Timer::Pause()
{
	_running = false;
}

void Timer::Refresh()
{
	std::chrono::time_point<std::chrono::system_clock>	_current = std::chrono::system_clock::now();
	size_t remain = _running ? (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(_current - _saved).count() : 0;

	_saved = _current;
	this->AddToTimer(remain);
}

void	Timer::AddToTimer(size_t value)
{
	size_t	minutes;
	size_t	seconds;
	size_t	miliseconds;

	minutes = value / 60000;
	seconds = value / 1000;
	miliseconds = value % 1000;

	_Timer.Minutes = _Timer.Minutes + minutes;
	_Timer.Seconds = _Timer.Seconds + seconds + (_Timer.Miliseconds + miliseconds) / 1000;
	_Timer.Miliseconds = (_Timer.Miliseconds + miliseconds) % 1000;

	if (_Timer.Seconds >= 60)
	{
		_Timer.Minutes += (_Timer.Seconds / 60);
		_Timer.Seconds %= 60;
	}
}

void Timer::SetTimer(size_t mins, size_t secs, size_t milisecs)
{
	_Timer.Minutes		= mins;
	_Timer.Seconds		= secs;
	_Timer.Miliseconds	= milisecs;
}

void Timer::ResetTimer()
{
	SetTimer(0, 0, 0);
}

const Time	&Timer::GetTimer() const
{
	return (_Timer);
}

const std::string Timer::GetTimerString()
{
	std::string min = std::to_string(_Timer.Minutes);
	std::string sec = std::to_string(_Timer.Seconds);
	std::string mili = std::to_string(_Timer.Miliseconds);

	Refresh();
	return (std::string(std::max(0, 2 - static_cast<int>(min.size())), '0') + min + ":" +
			std::string(std::max(0, 2 - static_cast<int>(sec.size())), '0') + sec + ":" +
			std::string(std::max(0, 3 - static_cast<int>(mili.size())), '0') + mili);
}

bool	Time::operator<(const Time &oth) const
{
	return ((this->Minutes <= oth.Minutes) && (this->Seconds <= oth.Seconds) && (this->Miliseconds < oth.Miliseconds));
}

bool	Time::operator<=(const Time &oth) const
{
	return ((this->Minutes <= oth.Minutes) && (this->Seconds <= oth.Seconds) && (this->Miliseconds <= oth.Miliseconds));
}

bool	Time::operator>(const Time &oth) const
{
	return ((this->Minutes >= oth.Minutes) && (this->Seconds >= oth.Seconds) && (this->Miliseconds > oth.Miliseconds));
}

bool	Time::operator>=(const Time &oth) const
{
	return ((this->Minutes >= oth.Minutes) && (this->Seconds >= oth.Seconds) && (this->Miliseconds >= oth.Miliseconds));
}

bool	Time::operator==(const Time &oth) const
{
	return ((this->Minutes == oth.Minutes) && (this->Seconds == oth.Seconds) && (this->Miliseconds == oth.Miliseconds));
}

bool	Time::operator!=(const Time &oth) const
{
	return (!(this->Minutes == oth.Minutes) && (this->Seconds == oth.Seconds) && (this->Miliseconds == oth.Miliseconds));
}

const Time	&Time::operator/=(int value)
{
	this->Miliseconds /= value;
	this->Seconds /= value;
	this->Minutes /= value;
	return *this;
}

const Time	&Time::operator*=(int value)
{
	this->Miliseconds *= value;
	this->Seconds *= value;
	this->Minutes *= value;
	return *this;
}