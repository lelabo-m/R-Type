#pragma once
#include <string>
#include <map>

enum keysOpt
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	FIRE
};

class gameOptions
{
private:
	std::map<keysOpt, std::string>		_keys;
	std::string		_nickName;
	int				_volumeBackground;
	int				_volumeSound;

private:
	static gameOptions *inst;

private:
	const gameOptions &operator=(const gameOptions &) { return *this; };
	const gameOptions &operator=(const gameOptions &&) { return *this; };
	gameOptions(const gameOptions &);
	gameOptions();
	~gameOptions();

public:
	static gameOptions *getGameOptions();
	static void			deleteGameOptions();

public:
	bool	hasKey(const std::string &) const;
	void	setKey(keysOpt, const std::string &);

public:
	const std::string	&getKeyUp() const;
	const std::string	&getKeyDown() const;
	const std::string	&getKeyLeft() const;
	const std::string	&getKeyRight() const;
	const std::string	&getKeyFire() const;
	const std::string	&getNickName() const;
	int					getVolumeBackground() const;
	int					getVolumeSound() const;

	void	setKeyUp(const std::string &);
	void	setKeyDown(const std::string &);
	void	setKeyLeft(const std::string &);
	void	setKeyRight(const std::string &);
	void	setKeyFire(const std::string &);
	void	setNickName(const std::string &);
	void	setVolumeBackground(int);
	void	setVolumeSound(int);
};