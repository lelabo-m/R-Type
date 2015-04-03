#include "SFMLoptionMenu.h"
#include "databaseSFML.h"
#include "SFMLbuttonevents.h"
#include "SFMLbasicText.h"
#include "Playlist.hh"
#include "gameOptions.h"

SFMLoptionMenu::SFMLoptionMenu()
{
	this->_menuName = "Options";
	this->initBaseOptionMenu();
}


SFMLoptionMenu::~SFMLoptionMenu()
{

	delete 	_backVolume;
	delete 	_soundVolume;
	delete	_nickName;

	delete	_keyUp;
	delete 	_keyLeft;
	delete 	_keyDown;
	delete 	_keyRight;
	delete 	_keyFire;

	delete	_cancel;
	delete 	_confirm;

}

void	SFMLoptionMenu::initOptions()
{
	gameOptions		*opts = gameOptions::getGameOptions();

	_backVolume		= new SFMLbasicSlider(0, 100, opts->getVolumeBackground(), sf::Vector2<int>(500, 147));
	_soundVolume	= new SFMLbasicSlider(0, 100, opts->getVolumeSound(), sf::Vector2<int>(500, 207));
	_nickName		= new SFMLbasicInput(20, sf::Vector2<int>(500, 260), opts->getNickName());

	_keyUp		= new SFMLcharacterInput(sf::Vector2<int>(300, 410), keysOpt::UP,opts->getKeyUp());
	_keyLeft	= new SFMLcharacterInput(sf::Vector2<int>(300, 460), keysOpt::LEFT, opts->getKeyLeft());
	_keyDown	= new SFMLcharacterInput(sf::Vector2<int>(300, 510), keysOpt::DOWN,opts->getKeyDown());
	_keyRight	= new SFMLcharacterInput(sf::Vector2<int>(300, 560), keysOpt::RIGHT,opts->getKeyRight());
	_keyFire	= new SFMLcharacterInput(sf::Vector2<int>(300, 610), keysOpt::FIRE,opts->getKeyFire());

	_backVolume->_funcValueChanged = std::bind(&SFMLoptionMenu::changeBackSound, this, _backVolume);

	this->_texts.push_back(SFMLbasicText("Options", sf::Vector2<int>(580, 50)));
	this->_texts.push_back(SFMLbasicText("Volume background", sf::Vector2<int>(172, 135)));
	this->_texts.push_back(SFMLbasicText("Volume sounds", sf::Vector2<int>(172, 195)));
	this->_texts.push_back(SFMLbasicText("Nickname", sf::Vector2<int>(172, 255)));
	this->_texts.push_back(SFMLbasicText("Input options", sf::Vector2<int>(542, 340)));

	this->_texts.push_back(SFMLbasicText("Up :", sf::Vector2<int>(172, 405)));
	this->_texts.push_back(SFMLbasicText("Left :", sf::Vector2<int>(172, 455)));
	this->_texts.push_back(SFMLbasicText("Down :", sf::Vector2<int>(172, 505)));
	this->_texts.push_back(SFMLbasicText("Right :", sf::Vector2<int>(172, 555)));
	this->_texts.push_back(SFMLbasicText("Fire :", sf::Vector2<int>(172, 605)));

	this->_forms.push_back(_backVolume);
	this->_eventManager.add(_backVolume);

	this->_forms.push_back(_soundVolume);
	this->_eventManager.add(_soundVolume);

	this->_forms.push_back(_nickName);
	this->_eventManager.add(_nickName);

	this->_forms.push_back(_keyUp);
	this->_eventManager.add(_keyUp);

	this->_forms.push_back(_keyLeft);
	this->_eventManager.add(_keyLeft);

	this->_forms.push_back(_keyDown);
	this->_eventManager.add(_keyDown);

	this->_forms.push_back(_keyRight);
	this->_eventManager.add(_keyRight);

	this->_forms.push_back(_keyFire);
	this->_eventManager.add(_keyFire);
}

void	SFMLoptionMenu::initBaseOptionMenu()
{
	databaseSFML	*db	= databaseSFML::getDatabase();

	_confirm	= new SFMLbigButton("Confirm", sf::Vector2<int>(115, 762));
	_cancel		= new SFMLbigButton("Cancel", sf::Vector2<int>(444, 762));

	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));
	backgroundSubmenu.setPosition(140.0, 40.0);

	_cancel->_funcRelease = std::bind(&SFMLoptionMenu::Cancel, this, _cancel);
	_confirm->_funcRelease = std::bind(&SFMLoptionMenu::savingOptions, this, _confirm);
	this->_images.push_back(backgroundSubmenu);

	this->initOptions();

	this->_forms.push_back(_confirm);
	this->_eventManager.add(_confirm);

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void	SFMLoptionMenu::savingOptions(SFMLshape	*)
{
	gameOptions		*opts = gameOptions::getGameOptions();

	opts->setKeyDown(_keyDown->getMessage());
	opts->setKeyUp(_keyUp->getMessage());
	opts->setKeyLeft(_keyLeft->getMessage());
	opts->setKeyRight(_keyRight->getMessage());
	opts->setKeyFire(_keyFire->getMessage());
	opts->setVolumeBackground(_backVolume->getValue());
	opts->setVolumeSound(_soundVolume->getValue());
	opts->setNickName(_nickName->getMessage());

	this->_menuManager->goTo("Main");
}

void	SFMLoptionMenu::Cancel(SFMLshape *)
{
	Playlist		*pl = Playlist::getPlaylist();
	gameOptions		*opts = gameOptions::getGameOptions();

	pl->setVolumePlaylist(static_cast<float>(opts->getVolumeBackground()) / 100);

	this->_menuManager->goTo("Main");
}

void	SFMLoptionMenu::changeBackSound(SFMLslider *sender)
{
	Playlist	*pl = Playlist::getPlaylist();
	
	pl->setVolumePlaylist(static_cast<float>(sender->getValue()) / 100);
}
