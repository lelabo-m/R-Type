#include <iostream>
#include <sstream>
#include "SFMLlauncherMenu.h"
#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "SFMLmainMenu.h"
#include "databaseSFML.h"

SFMLlauncherMenu::SFMLlauncherMenu()
{
	this->_menuName = "Launcher";
	this->_isConnected = false;
	this->initBaseLauncher();
	this->initLauncher();
}

SFMLlauncherMenu::~SFMLlauncherMenu()
{
	delete _ip;
	delete _connect;
	delete _cancel;
}

void	SFMLlauncherMenu::initLauncher()
{
	this->_ip		= new SFMLinputLauncher(sf::Vector2<int>(430, 320));
	this->_connect	= new SFMLbuttonLauncher(sf::Vector2<int>(430, 572), "Connect");
	this->_cancel	= new SFMLbuttonLauncher(sf::Vector2<int>(668, 572), "Cancel");

	this->_cancel->_funcRelease = std::bind(&SFMLlauncherMenu::cancel, this, _cancel);
	this->_connect->_funcRelease = std::bind(&SFMLlauncherMenu::connect, this, _connect);
	this->_ip->_funcValidInput = std::bind(&SFMLlauncherMenu::connect, this, _connect);

	this->_forms.push_back(_ip);
	this->_forms.push_back(_connect);
	this->_forms.push_back(_cancel);

	this->_eventManager.add(_ip);
	this->_eventManager.add(_connect);
	this->_eventManager.add(_cancel);
}

void	SFMLlauncherMenu::initBaseLauncher()
{
	SFMLbasicText	title("R-Type launcher", sf::Vector2<int>(520, 75), 30);
	SFMLbasicText	ip("IP", sf::Vector2<int>(630, 270), 20);

	this->_texts.push_back(title);
	this->_texts.push_back(ip);
}

void	SFMLlauncherMenu::connect(SFMLshape *)
{
	SFMLmainMenu	*main = new SFMLmainMenu();
	NetClient		*nc = &(_client->getNetClient());

	try
	{
		nc->connect(_ip->getMessage(), 6666);
		if (nc->getConnectionState() == NetClientManager::CONNECTED)
			this->_isConnected = true;
		_menuManager->goTo("Main");
		main->launch();
	}
	catch (const std::runtime_error &err)
	{
		std::cerr << "Fail on connecting" << std::endl;
		std::cerr << "Error : " << err.what() << std::endl;
	}

	delete main;
	if (this->_isConnected)
	{
		nc->disconnect();
		this->_isConnected = false;
	}
}

void	SFMLlauncherMenu::executeRequests()
{

}

void	SFMLlauncherMenu::cancel(SFMLshape *)
{
	_menuManager->goTo("");
}
