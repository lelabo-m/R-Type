#include "SFMLlevelscoreMenu.h"
#include "databaseSFML.h"
#include "SFMLbigButton.h"
#include "SFMLbasicText.h"
#include "SFMLbuttonevents.h"

SFMLlevelscoreMenu::SFMLlevelscoreMenu(const std::string &levelName)
{
	this->_menuName = "LevelScore";
	_client->Scores.clear();
	this->_client->askMapScore(levelName + ".map");
	this->initBaseLevelscoreMenu(levelName);
}

SFMLlevelscoreMenu::~SFMLlevelscoreMenu()
{
	delete 	_cancel;
}

void	SFMLlevelscoreMenu::initBaseLevelscoreMenu(const std::string &levelName)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	SFMLbasicText	title("Ranking for " + levelName, sf::Vector2<int>());
	sf::Sprite		backgroundSubmenu(db->getTexture("backgroundSubmenu"));

	_cancel = new SFMLbigButton("Return", sf::Vector2<int>(350, 762));

	backgroundSubmenu.setPosition(140.0, 40.0);
	title.setPosition(sf::Vector2<float>(640 - title.getLocalBounds().width / 2, 55));

	_cancel->_funcRelease = std::bind(&SFMLlevelscoreMenu::Cancel, this, _cancel);
	this->_images.push_back(backgroundSubmenu);
	this->_texts.push_back(title);

	this->_forms.push_back(_cancel);
	this->_eventManager.add(_cancel);
}

void	SFMLlevelscoreMenu::Cancel(SFMLshape *)
{
	_menuManager->goTo("Main");
}

void	SFMLlevelscoreMenu::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());
	
	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}

	if (_bestScores.empty() && !_client->Scores.empty())
	{
		int i = 1;

		for (const auto &elem : _client->Scores)
		{
			_bestScores.push_back(SFMLscore("#" + std::to_string(i), elem.first, elem.second, sf::Vector2<int>(170, 90 + (50 * i)), 30));
			i++;
		}

		_bestScores.sort([](const SFMLscore &left, const SFMLscore &right) { return left.getScore() < right.getScore(); });
	}
}

void	SFMLlevelscoreMenu::displayDynaicContents()
{
	int		i = 0;

	for (auto &elem : _bestScores)
	{
		if (i >= 10)
			break;
		elem.Draw(*_window);
		i++;
	}
}