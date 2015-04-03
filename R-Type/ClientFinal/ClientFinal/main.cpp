#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLmainMenu.h"
#include "Playlist.hh"
#include "EventManager.h"
#include "databaseSFML.h"
#include "SFMLlauncherMenu.h"
#include "gameOptions.h"
#include "Controller.hpp"

void	initComponents()
{
	databaseSFML	*db = databaseSFML::getDatabase();
	Playlist		*pl = Playlist::getPlaylist();

	sf::Texture img;
	sf::Font	fnt;

	std::vector<std::string>	files;
	FileSystem::ListDirectory(DATABASE_PATH, "*", files);

	for (const auto &val : files)
	{
		if (FileSystem::FileExt(val) == ".png" && img.loadFromFile(val))
			db->addTexture(FileSystem::FileShortName(val), img);
		if (FileSystem::FileExt(val) == ".otf" && fnt.loadFromFile(val))
			db->addFont(FileSystem::FileShortName(val), fnt);
		if (FileSystem::FileExt(val) == ".mp3")
			pl->loadSound(val, FileSystem::FileShortName(val));
	}
}

void	initKeyBoard()
{
	KeyBoard			*keyboard = new KeyBoard();
	ControllerManager	*cm = new ControllerManager();
	int			i = sf::Keyboard::A;
	int			j = sf::Keyboard::A;

	while (i <= sf::Keyboard::KeyCount)
	{
		keyboard->Bind(i, i);
		++i;
	}

	while (j <= sf::Keyboard::Num9)
	{
		keyboard->Bind(j, i);
		keyboard->Bind(sf::Keyboard::LShift, i);
		++i;
		++j;
	}

	j = sf::Keyboard::LBracket;
	while (j <= sf::Keyboard::Dash)
	{
		keyboard->Bind(j, i);
		keyboard->Bind(sf::Keyboard::LShift, i);
		++i;
		++j;
	}

	cm->AddController(keyboard);
}

int main()
{
#ifndef _WIN32
	if (getenv("DISPLAY") == nullptr)
	{
		std::cerr << "No environment found" << std::endl;
		return -1;
	}
#else
	FreeConsole();

#endif

	initComponents();
	initKeyBoard();

	SFMLlauncherMenu	launcher;
	Playlist			*pl = Playlist::getPlaylist();

	pl->playSound("mainSound");
	try
	{
		launcher.launch();
	}
	catch (const ReturnToMenu &)
	{
	}
	catch (const DatabaseException &err)
	{
		std::cerr << err.what() << std::endl;
	}

	ControllerManager::Kill();
	databaseSFML::deleteDatabase();
	windowSFML::deleteWindow();
	gameOptions::deleteGameOptions();
	Playlist::delPlaylist();
	return 0;
}
