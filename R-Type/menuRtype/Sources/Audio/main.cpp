#include <unistd.h>
#include "Playlist.hh"
#include "Thread.hh"
#include <iostream>

void	*func(void *param)
{
	Playlist *SoundContent = Playlist::getPlaylist();
	Sound *sound;
	Sound *sound2;
	//Sound *sound3;


	param = param;

	SoundContent->loadSound("sounds/Envy -  Paladin.mp3", "Paladin");
	SoundContent->loadSound("sounds/Feint - Sky Dance.mp3", "SkyDance");
	SoundContent->loadSound("sounds/Feint &amp; Boyinaband - Time Bomb (feat. Veela).mp3", "TimeBomb");
	SoundContent->loadSound("sounds/Zard.mp3", "Zard");
	SoundContent->loadSound("sounds/pipe.wav", "SkyDances");

	SoundContent->playSound("SkyDance");
	SoundContent->playSound("TimeBomb");
	SoundContent->playSound("Zard");
	SoundContent->playSound("Paladin");
	SoundContent->playSound("TimeBomb");
	SoundContent->playSound("twoll");
	sound = SoundContent->getMusic("twoll");
	sound2 = SoundContent->getMusic("SkyDance");
	sound = SoundContent->getMusic("Paladin");
	//sound3 = SoundContent->getMusic("TimeBomb");
	sleep(3);
	SoundContent->pausePlaylist();
	sleep(1);
	sound->unpauseSound();
	sound2->unpauseSound();
	//sound3->pauseSound();
	/* code */
	sleep(5);
	SoundContent->setVolumePlaylist(0.5);
	sleep(2);
	sound2->setVolume(1);
	sleep(2);
	sound2->setVolume(0.1);
	sleep(2);
	sound2->setVolume(2);
	sleep(2);
	sound2->setVolume(0);
	sleep(2);
	std::cout << "END" << std::endl;
	Playlist::delPlaylist();
	return 0;
}

int	main()
{
  Thread	th(func, 0);

  th.run();
  th.join();
}
