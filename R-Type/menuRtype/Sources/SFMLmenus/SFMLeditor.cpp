#define NOMINMAX
#include <iostream>
#include <sstream>
#include <algorithm>
#include "SFMLeditor.h"
#include "SFMLbuttonevents.h"
#include "SFMLlengthlevelPopUp.h"
#include "SFMLbasicText.h"
#include "Level.hh"
#include "InputPopupValue.h"
#include "databaseSFML.h"

SFMLeditor::SFMLeditor(const std::string &name)
{
	this->_mapName = name;
	this->_menuName = "Editor";
	this->initEditor();
	this->initMonsterList();
}

SFMLeditor::~SFMLeditor()
{
	delete	_save;
	delete	_quit;
	delete	_length;

	delete	_up;
	delete	_down;

	delete	_play;
	delete	_pause;
	delete	_stop;
	delete	_timeline;

	this->_client->clearGraphicalElements();
}

void	SFMLeditor::initMonsterList()
{
	Level			lvl;
	SFMLmonsterDnD	*monster;
	sf::IntRect		area;
	Timer			time;
	Time			_spawnTime;

	if (lvl.readFromFile("maps" + std::string(SLASHSEP) + this->_mapName) == true)
	{
		for (const auto &elem : lvl.getLevelElems())
		{
			
			time.SetTimer(elem.time / (1000 * 60), (elem.time / 1000) % 60, elem.time % 1000);
			_spawnTime = time.GetTimer();

			for (const auto &graphElem : this->_monsterList)
			{
				if (graphElem->getName() == elem.name)
				{	
					monster = new SFMLmonsterDnD(*(graphElem->_background.getTexture()), elem.name, sf::Vector2<int>(elem.posX, elem.posY), _spawnTime);
					area = monster->getPosition();
					area.width	= graphElem->getSizeSprite().x;
					area.height = graphElem->getSizeSprite().y;

					monster->setPosition(area);
					this->_monsters.push_back(monster);
				}
			}
			
		}
	}
}

void	SFMLeditor::initMenu()
{
	SFMLbasicText	title("R-Type Editor", sf::Vector2<int>(880, 10));

	_save		= new SFMLeditormenuButton("Save file", sf::Vector2<int>(10, 10));
	_length		= new SFMLeditormenuButton("Set length", sf::Vector2<int>(150, 10));
	_quit		= new SFMLeditormenuButton("Quit", sf::Vector2<int>(290, 10));

	_quit->_funcRelease = std::bind(&SFMLeditor::Quit, this, _quit);
	_length->_funcRelease = std::bind(&SFMLeditor::defineLengthLevel, this, _length);
	_save->_funcRelease = std::bind(&SFMLeditor::saveFile, this, _save);

	this->_forms.push_back(_save);
	this->_forms.push_back(_length);
	this->_forms.push_back(_quit);

	this->_texts.push_back(title);
}

void	SFMLeditor::initSpriteList()
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_up = new SFMLbutton("", db->getTexture("upArrow"), sf::Vector2<int>(0, 125));
	_down = new SFMLbutton("", db->getTexture("downArrow"), sf::Vector2<int>(0, 725));

	_up->_funcFocus		= std::bind(SFMLbuttonevents::upArrowHover, _up);
	_up->_funcUnfocus	= std::bind(SFMLbuttonevents::upArrowUnhover, _up);
	_up->_funcRelease	= std::bind(&SFMLeditor::upButton, this, _up);
	_down->_funcFocus	= std::bind(SFMLbuttonevents::downArrowHover, _down);
	_down->_funcUnfocus = std::bind(SFMLbuttonevents::downArrowUnhover, _down);
	_down->_funcRelease	= std::bind(&SFMLeditor::downButton, this, _down);

	this->_forms.push_back(_up);
	this->_forms.push_back(_down);

	std::list<GraphicalElem>	elems = _client->getGraphicalElements();
	int							i = 0;

	for (auto &elem : elems)
	{
		SFMLanimation	*animation = elem.GetAnimation();
		if (!animation)
		{
			elem.Play(0, true);
			animation = elem.GetAnimation();
		}
		if (animation)
		{
			sf::Rect<int>	area = animation->getPosition();

			area.left = 60 - (area.width / 2);
			area.top = (110 * i) + 225 - (area.height / 2);
			animation->setPosition(area);
			animation->_funcRelease = std::bind(&SFMLeditor::createMonster, this, animation);

			this->_monsterList.push_back(animation);
			i++;
		}
	}
}

void	SFMLeditor::initTimer()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	_time		= SFMLbasicText(_timer.GetTimerString(), sf::Vector2<int>(20,845), 20);

	_play		= new SFMLbutton("", db->getTexture("playButton"), sf::Vector2<int>(1135, 840));
	_pause		= new SFMLbutton("", db->getTexture("pauseButton"), sf::Vector2<int>(1185, 840));
	_stop		= new SFMLbutton("", db->getTexture("stopButton"), sf::Vector2<int>(1235, 840));
	_timeline	= new SFMLslider(0, _secsTimeline * 1000, db->getTexture("sliderTimerBar"), db->getTexture("sliderTimerTick"), sf::Vector2<int>(227, 840));

	_play->_funcFocus	= std::bind(SFMLbuttonevents::playButtonHover, _play);
	_play->_funcUnfocus	= std::bind(SFMLbuttonevents::playButtonUnhover, _play);
	_play->_funcRelease = std::bind(&SFMLeditor::play, this, _play);

	_pause->_funcFocus		= std::bind(SFMLbuttonevents::pauseButtonHover, _pause);
	_pause->_funcUnfocus	= std::bind(SFMLbuttonevents::pauseButtonUnhover, _pause);
	_pause->_funcRelease	= std::bind(&SFMLeditor::pause, this, _pause);

	_stop->_funcFocus = std::bind(SFMLbuttonevents::stopButtonHover, _stop);
	_stop->_funcUnfocus = std::bind(SFMLbuttonevents::stopButtonUnhover, _stop);
	_stop->_funcRelease = std::bind(&SFMLeditor::stop, this, _stop);

	_timeline->_funcValueChanged = std::bind(&SFMLeditor::setTimer, this, _timeline);

	this->_forms.push_back(_play);
	this->_forms.push_back(_pause);
	this->_forms.push_back(_stop);
	this->_forms.push_back(_timeline);
}

void	SFMLeditor::initEditor()
{
	this->_isPlaying = false;
	this->_secsTimeline = 30;
	this->initMenu();
	this->initSpriteList();
	this->initTimer();
}

void	SFMLeditor::upButton(SFMLshape *)
{
	SFMLanimation		*subject;
	sf::Rect<int>		area;

	if (this->_monsterList.size() > 1)
	{
		subject = this->_monsterList.front();
		subject->setPosition(this->_monsterList.back()->getPosition());

		this->_monsterList.pop_front();
		for (auto &monster : this->_monsterList)
		{
			area = monster->getPosition();
			area.top -= 110;
			monster->setPosition(area);
		}
		this->_monsterList.push_back(subject);
	}
}

void	SFMLeditor::downButton(SFMLshape *)
{
	SFMLanimation	*subject;
	sf::Rect<int>	area;

	if (this->_monsterList.size() > 1)
	{
		subject = this->_monsterList.back();
		subject->setPosition(this->_monsterList.front()->getPosition());
		
		this->_monsterList.pop_back();
		for (auto &monster : this->_monsterList)
		{
			area = monster->getPosition();
			area.top += 110;
			monster->setPosition(area);
		}
		this->_monsterList.push_front(subject);
	}
}

void	SFMLeditor::createMonster(SFMLshape *sender)
{
	static int posY = 100;
	const sf::Texture	*mTexture	= sender->_background.getTexture();
	sf::IntRect	area				= sender->_background.getTextureRect();
	SFMLanimation *animation		= dynamic_cast<SFMLanimation *>(sender);

	if (animation && mTexture)
	{
		SFMLmonsterDnD	*monster = new SFMLmonsterDnD(*mTexture, animation->getName(), sf::Vector2<int>(1201 - (area.width / 2), posY - (area.height / 2)), _timer.GetTimer());

		area.top = posY - (area.height / 2);
		monster->setPosition(area);
		this->_monsters.push_back(monster);
		posY = (posY % 800) + 100;
	}
}

void	SFMLeditor::setTimer(SFMLslider *sender)
{
	int value = sender->getValue();

	this->_timer.SetTimer(value / (1000 * 60), (value / 1000) % 60, value % 1000);

}

void	SFMLeditor::play(SFMLshape *)
{
	this->_isPlaying = true;
	_timer.Start();
}

void	SFMLeditor::pause(SFMLshape *)
{
	this->_isPlaying = false;
	_timer.Pause();
}

void	SFMLeditor::stop(SFMLshape *)
{
	this->_isPlaying = false;
	_timer.Stop();
}

void	SFMLeditor::defineLengthLevel(SFMLshape *)
{
	SFMLInputPopUp		popUp("R-Type editor", "Define your level length (in seconds)", 3);
	InputPopupValue		*inputPopup = InputPopupValue::getInst();

	this->_menuManager->goTo("InputPopUp");
	popUp.launch();
	std::string		retVal(inputPopup->getValue());
	int				value;

	if (retVal != "")
	{
		std::istringstream ss(retVal);
		ss >> value;

		this->_secsTimeline = std::max(value, 30);
		this->_timeline->setMaxValue(this->_secsTimeline * 1000);
		_timer.SetTimer(this->_secsTimeline / 60, this->_secsTimeline % 60, 0);

		this->_monsters.remove_if([this](SFMLmonsterDnD *obj) {return obj->getTimer() > _timer.GetTimer(); });
	}
}

void	SFMLeditor::updateDynamicContents(const sf::Event &)
{

}

void	SFMLeditor::updateContent()
{
	const Time		t = _timer.GetTimer();
	long int		timeGiven = (t.Minutes * 60 * 1000) + (t.Seconds * 1000) + t.Miliseconds;

	_timeline->updateSlider(timeGiven);
	if (timeGiven >= _secsTimeline * 1000)
		this->pause(nullptr);

	this->_time.setString(_timer.GetTimerString());
	this->_monsters.remove_if([](SFMLmonsterDnD *obj) {return obj->hasToDelete(); });
}

void	SFMLeditor::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
	if (this->_monsterList.empty())
	{
		std::list<GraphicalElem>	elems = _client->getGraphicalElements();
		int							i = 0;

		for (auto &elem : elems)
		{
			SFMLanimation	*animation = elem.GetAnimation();
			if (!animation)
			{
				elem.Play(1, true);
				animation = elem.GetAnimation();
			}
			if (animation)
			{
				sf::Rect<int>	area = animation->getPosition();

				area.left = 60 - (area.width / 2);
				area.top = (110 * i) + 225 - (area.height / 2);
				animation->setPosition(area);
				animation->_funcRelease = std::bind(&SFMLeditor::createMonster, this, animation);

				this->_monsterList.push_back(animation);
				i++;
			}
		}
	}
}

void	SFMLeditor::displayDynamicContents()
{
	int		count;

	count = 0;
	for (const auto &monster : this->_monsterList)
	{
		if (count >= 5)
			break;
		monster->Draw(*this->_window);
		++count;
	}
	for (auto &monster : this->_monsters)
	{
		monster->checkVisibility(_timer.GetTimer());
		if (monster->isVisible())
		{
			monster->updateXPosition(_timer.GetTimer());
			monster->Draw(*this->_window);
		}
	}
}

void	SFMLeditor::saveFile(SFMLshape *)
{
	std::list<LevelElem>	_listMonsters;
	std::list<LevelRef>		_listRefs;
	std::string				_content;
	sf::Vector2<int>		_spawnPos;
	Time					_spawnTime;

	for (const auto &monster : this->_monsters)
	{
		_spawnPos = monster->getSpawnPos();
		_spawnTime = monster->getTimer();
		_listMonsters.push_back(LevelElem(monster->getName(), _spawnPos.x, _spawnPos.y,
								(_spawnTime.Miliseconds) +
								(_spawnTime.Seconds * 1000) +
								(_spawnTime.Minutes * 60 * 1000)));
	}

	Level level(_listRefs, _listMonsters);

	std::cout << "Saving on " << this->_mapName << std::endl;
	level.writeToFile("maps" + std::string(SLASHSEP) + this->_mapName);

	_client->askSendMap(this->_mapName + ".map");
}

void	SFMLeditor::Quit(SFMLshape *)
{
	_menuManager->goTo("Main");
}