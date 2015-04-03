#include <SFML/Graphics.hpp>
#include "Playlist.hh"
#include "EventManager.h"
#include "databaseSFML.h"
#include "SFMLmainMenu.h"
#include "SFMLlauncherMenu.h"
#include "gameOptions.h"

void	initComponents()
{
	databaseSFML	*db = databaseSFML::getDatabase();
	Playlist		*pl = Playlist::getPlaylist();

	sf::Texture img;
	sf::Font	fnt;

	pl->loadSound("Others/Kalimba.mp3", "menuTheme");

	if (img.loadFromFile("Others/backgroundMenu.png"))		db->addTexture("backgroundMenu", img);
	if (img.loadFromFile("Others/backgroundSubmenu.png"))	db->addTexture("backgroundSubmenu", img);
	if (img.loadFromFile("Others/backgroundEditor.png"))	db->addTexture("backgroundEditor", img);
	if (img.loadFromFile("Others/backgroundLauncher.png"))	db->addTexture("backgroundLauncher", img);
	if (img.loadFromFile("Others/borderMenu.png"))			db->addTexture("borderMenu", img);

	if (img.loadFromFile("Others/littleButton.png"))		db->addTexture("littleButton", img);
	if (img.loadFromFile("Others/littleButtonHover.png"))	db->addTexture("littleButtonHover", img);
	if (img.loadFromFile("Others/littleButtonClick.png"))	db->addTexture("littleButtonClick", img);

	if (img.loadFromFile("Others/mapselectorButton.png"))		db->addTexture("mapselectorButton", img);
	if (img.loadFromFile("Others/mapselectorButtonHover.png"))	db->addTexture("mapselectorButtonHover", img);
	if (img.loadFromFile("Others/mapselectorButtonClick.png"))	db->addTexture("mapselectorButtonClick", img);

	if (img.loadFromFile("Others/submenuButton.png"))		db->addTexture("submenuButton", img);
	if (img.loadFromFile("Others/submenuButtonHover.png"))	db->addTexture("submenuButtonHover", img);
	if (img.loadFromFile("Others/submenuButtonClick.png"))	db->addTexture("submenuButtonClick", img);

	if (img.loadFromFile("Others/bigButton.png"))		db->addTexture("bigButton", img);
	if (img.loadFromFile("Others/bigButtonHover.png"))	db->addTexture("bigButtonHover", img);
	if (img.loadFromFile("Others/bigButtonClick.png"))	db->addTexture("bigButtonClick", img);

	if (img.loadFromFile("Others/menueditorButton.png"))		db->addTexture("editorButton", img);
	if (img.loadFromFile("Others/menueditorButtonHover.png"))	db->addTexture("editorButtonHover", img);
	if (img.loadFromFile("Others/upArrow.png"))					db->addTexture("upArrow", img);
	if (img.loadFromFile("Others/upArrowHover.png"))			db->addTexture("upArrowHover", img);
	if (img.loadFromFile("Others/downArrow.png"))				db->addTexture("downArrow", img);
	if (img.loadFromFile("Others/downArrowHover.png"))			db->addTexture("downArrowHover", img);

	if (img.loadFromFile("Others/playButton.png"))			db->addTexture("playButton", img);
	if (img.loadFromFile("Others/pauseButton.png"))			db->addTexture("pauseButton", img);
	if (img.loadFromFile("Others/stopButton.png"))			db->addTexture("stopButton", img);
	if (img.loadFromFile("Others/playButtonHover.png"))		db->addTexture("playButtonHover", img);
	if (img.loadFromFile("Others/pauseButtonHover.png"))	db->addTexture("pauseButtonHover", img);
	if (img.loadFromFile("Others/stopButtonHover.png"))		db->addTexture("stopButtonHover", img);
	if (img.loadFromFile("Others/sliderTimerBar.png"))		db->addTexture("sliderTimerBar", img);
	if (img.loadFromFile("Others/sliderTimerTick.png"))		db->addTexture("sliderTimerTick", img);

	if (img.loadFromFile("Others/sliderTick.png"))		db->addTexture("sliderTick", img);
	if (img.loadFromFile("Others/sliderBar.png"))		db->addTexture("sliderBar", img);

	if (img.loadFromFile("Others/monsterButtonHover.png"))		db->addTexture("monsterButtonHover", img);
	if (img.loadFromFile("Others/monsterButton.png"))			db->addTexture("monsterButton", img);

	if (img.loadFromFile("Others/popupLauncher.png"))				db->addTexture("popupLauncher", img);
	if (img.loadFromFile("Others/popupInputBackgroundHover.png"))	db->addTexture("popupInputBackgroundHover", img);
	if (img.loadFromFile("Others/popupInputBackground.png"))		db->addTexture("popupInputBackground", img);
	if (img.loadFromFile("Others/popupButtonBackgroundHover.png"))	db->addTexture("popupButtonBackgroundHover", img);
	if (img.loadFromFile("Others/popupButtonBackground.png"))		db->addTexture("popupButtonBackground", img);

	if (img.loadFromFile("Others/Sprite1.png"))		db->addTexture("Sprite1", img);
	if (img.loadFromFile("Others/Sprite2.png"))		db->addTexture("Sprite2", img);
	if (img.loadFromFile("Others/Sprite3.png"))		db->addTexture("Sprite3", img);
	if (img.loadFromFile("Others/Sprite4.png"))		db->addTexture("Sprite4", img);
	if (img.loadFromFile("Others/Sprite5.png"))		db->addTexture("Sprite5", img);

	if (fnt.loadFromFile("Others/Brawler.otf"))			db->addFont("Brawler", fnt);
}

int main()
{
	initComponents();

	SFMLlauncherMenu	launcher;

	try
	{
		launcher.launch();
	}
	catch (const ReturnToMenu &)
	{}

	databaseSFML::deleteDatabase();
	windowSFML::deleteWindow();
	gameOptions::deleteGameOptions();
	Playlist::delPlaylist();
	return 0;
}
