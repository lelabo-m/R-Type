#ifndef SOUND_HH
#define SOUND_HH

#include "fmod.h"

#define SOUND_FLAGS	FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM

class Sound
{
private:
	std::string		_name;
	FMOD_SOUND 		*_sound;
	FMOD_SYSTEM  	*_system;
	FMOD_CHANNEL	*_channel;

public:
	Sound(FMOD_SYSTEM *);
	~Sound();

	bool 	loadSound(const std::string &, const std::string &);
	void 	playSound();

	bool 	pauseSound();
	bool	unpauseSound();
	bool	setVolume(float);

	const std::string 	&getName() const;
	FMOD_SOUND 			*getSound() const;
	FMOD_CHANNEL		*getChannel() const;
};

#endif