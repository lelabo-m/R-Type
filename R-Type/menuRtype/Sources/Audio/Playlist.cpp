#include <iostream>
#include <algorithm>
#include "Playlist.hh"

Playlist *Playlist::_playlist = NULL;

Playlist::Playlist()
{
  if (FMOD_System_Create(&(this->_system)) != FMOD_OK)
    std::cerr << "Fail on creating fmod system" << std::endl;
  if (FMOD_System_Init(this->_system, 32, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
    std::cerr << "Fail on initialising fmod system" << std::endl;
}

Playlist::~Playlist()
{
  itListSounds	it;

  for (it = this->_musics.begin() ; it != this->_musics.end() ; ++it)
    delete *it;
  this->_musics.clear();
  if (this->_system)
    {
      FMOD_System_Close(this->_system);
      FMOD_System_Release(this->_system);
    }
}

Playlist	*Playlist::getPlaylist()
{
  if (!_playlist)
    _playlist =  new Playlist;
  return (_playlist);
}

void		Playlist::delPlaylist()
{
  if (_playlist)
    {
      delete (_playlist);
      _playlist = NULL;
    }
}

Sound 	*Playlist::getMusic(const std::string &name)
{
	itListSounds	it;

	for (it = this->_musics.begin() ; it != this->_musics.end() ; ++it)
		if ((*it)->getName() == name)
			return (*it);
	return (NULL);
}

bool	Playlist::loadSound(const std::string &path, const std::string &name)
{
  Sound*				content;

  content = this->getMusic(name);
  if (content)
    return (false);
  content = new Sound(this->_system);
  if (content->loadSound(path, name) == false)
    {
      delete content;
      return (false);
    }
  this->_musics.push_back(content);
  return (true);
}

bool	Playlist::playSound(const std::string &name)
{
  Sound*	content;

  content = this->getMusic(name);
  if (content)
    {
      content->playSound();
      return (true);
    }
  std::cerr << "Unfound music of name : " + name << std::endl;
  return (false);
}

void 	Playlist::pausePlaylist()
{
  itListSounds	it;

  for (it = this->_musics.begin() ; it != this->_musics.end() ; ++it)
    (*it)->pauseSound();
}

void	Playlist::unpausePlaylist()
{
  itListSounds	it;

  for (it = this->_musics.begin() ; it != this->_musics.end() ; ++it)
    (*it)->unpauseSound();
}

void	Playlist::setVolumePlaylist(float value)
{
  itListSounds	it;

  for (it = this->_musics.begin() ; it != this->_musics.end() ; ++it)
    (*it)->setVolume(value);
}
