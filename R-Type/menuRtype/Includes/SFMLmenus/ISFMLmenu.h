#ifndef ISFMLMENU_H_
# define ISFMLMENU_H_

# include <stdexcept>

# define NETWORK_TIMER 500 // Milliseconds
# define CYCLE_TIME_MENU 10000 // microseconds

class ReturnToMenu : public std::runtime_error
{
public:
	ReturnToMenu(const char *msg = "") : std::runtime_error(msg) {};
	ReturnToMenu(const ReturnToMenu& other) : std::runtime_error(other.what()) {}
	ReturnToMenu(const ReturnToMenu&& other) : std::runtime_error(other.what()) {}
	ReturnToMenu&	operator=(const ReturnToMenu&) { return *this; }
	ReturnToMenu&	operator=(ReturnToMenu&&) { return *this; }
};

class Disconnected : public std::runtime_error
{
public:
	Disconnected(const char *msg = "") : std::runtime_error(msg) {};
	Disconnected(const Disconnected& other) : std::runtime_error(other.what()) {}
	Disconnected(const Disconnected&& other) : std::runtime_error(other.what()) {}
	Disconnected&	operator=(const Disconnected&) { return *this; }
	Disconnected&	operator=(Disconnected&&) { return *this; }
};

class ISFMLmenu
{
public:
	ISFMLmenu() {};
	virtual ~ISFMLmenu() {};

public:
	virtual void	launch() = 0;
};

#endif // !ISFMLMENU_H_
