#include <iostream>
#include "Sound.hh"

Sound::Sound(FMOD_SYSTEM *sys)
{
	this->_sound 	= NULL;
	this->_channel 	= NULL;
	this->_system 	= sys;
}

Sound::~Sound()
{
  if (this->_sound)
    FMOD_Sound_Release(this->_sound);
}

/*
** Tester avec sys en const /!\
*/

bool 	Sound::loadSound(const std::string &path, const std::string &name)
{
  if (FMOD_System_CreateSound(this->_system, path.c_str(),
			      SOUND_FLAGS, NULL, &(this->_sound)) != FMOD_OK)
    {
      //std::cerr << std::string("Fail on creating fmod sound : ") << path << std::endl;
      return (false);
    }
  this->_name = name;
  return (true);
}

void 	Sound::playSound()
{
  FMOD_System_PlaySound(this->_system, FMOD_CHANNEL_FREE, this->_sound, 0, &(this->_channel));
}

bool	Sound::pauseSound()
{
  if (this->_channel)
    {
      FMOD_Channel_SetPaused(this->_channel, true);
      return (true);
    }
  return (false);
}

bool	Sound::unpauseSound()
{
  if (this->_channel)
    {
      FMOD_Channel_SetPaused(this->_channel, false);
      return (true);
    }
  return (false);
}

bool 	Sound::setVolume(float value)
{
  if (this->_channel)
    {
      if (FMOD_Channel_SetVolume(this->_channel, value))
	{
	  //std::cerr << "Error on setting volume sound " + this->_name << std::endl;
	  return (false);
	}
      return (true);
    }
  return (false);
}

const std::string &Sound::getName() const
{
  return (this->_name);
}

/*
** getter ci-dessus en const?
** (Vu que c'est des pointeurs je dirais non imo)
*/

FMOD_SOUND 		*Sound::getSound() const
{
  return (this->_sound);
}

FMOD_CHANNEL	*Sound::getChannel() const
{
  return (this->_channel);
}
