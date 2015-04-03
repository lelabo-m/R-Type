#ifndef Playlist_HH
#define Playlist_HH

#include <string>
#include <list>
#include "Sound.hh"

/*
  typedef std::map<std::string, FMOD_SOUND *>::iterator 			itListMusic;
  typedef std::map<std::string, FMOD_SOUND *>::const_iterator		 	itListMusicConst;
  typedef std::map<std::string, FMOD_SOUND *>					ListMusic;
  typedef std::list<FMOD_CHANNEL *>						ListChannels;
  typedef std::list<FMOD_CHANNEL *>::iterator 					itListChannels;
  typedef std::list<FMOD_CHANNEL *>::const_iterator 				itListChannelsConst;
*/

typedef std::list<Sound *> 			ListSounds;
typedef std::list<Sound *>::iterator 		itListSounds;
typedef std::list<Sound *>::const_iterator 	itListSoundsConst;

/* Singleton class */

class Playlist
{
public:
  static Playlist 	*getPlaylist();
  static void		delPlaylist();

  bool		loadSound(const std::string &, const std::string &);
  bool		playSound(const std::string &);
  Sound		*getMusic(const std::string &);
  void		pausePlaylist();
  void		unpausePlaylist();
  void		setVolumePlaylist(float value);
private:
	Playlist();
	~Playlist();
private:
  static Playlist 	*_playlist;
  FMOD_SYSTEM  		*_system;
  /*
    ListChannels 		_channels;
    ListMusic 			_musics;
  */
  ListSounds  		_musics;
  /* data */
};

#endif
