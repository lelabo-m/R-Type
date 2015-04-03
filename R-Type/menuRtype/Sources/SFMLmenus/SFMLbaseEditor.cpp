#include "SFMLbaseEditor.h"
#include "menuManager.h"
#include "databaseSFML.h"

SFMLbaseEditor::SFMLbaseEditor() : _eventManager(&this->_forms, &this->_monsterList, &this->_monsters)
{
	this->_menuName = "BaseEditor";
	this->_menuManager = menuManager::getInst();
	this->_window = windowSFML::getWindow();
	this->_client = ClientManager::getInstance();
	this->initBaseMenu();
}


SFMLbaseEditor::~SFMLbaseEditor()
{
	this->_images.clear();
	this->_texts.clear();
	this->_forms.clear();
	this->_monsterList.clear();
	for (auto &monster : this->_monsters)
		delete monster;
}

void	SFMLbaseEditor::initBaseMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sf::Sprite	background(db->getTexture("backgroundEditor"));

	this->_images.push_back(background);
}

void	SFMLbaseEditor::updateContent()
{

}

void	SFMLbaseEditor::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
}

void	SFMLbaseEditor::launch()
{
	sf::Event		 ev;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	while (_menuManager->getCurrentMenu() == this->_menuName)
	{

		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
		while (this->_window->pollEvent(ev))
		{
			this->_eventManager.execute(ev);
		}

		this->executeRequests();
		this->updateContent();

		this->_window->clear();
		for (const auto &image : this->_images)		this->_window->draw(image);
		for (const auto &text : this->_texts)		this->_window->draw(text);
		for (const auto &form : this->_forms)		form->Draw(*this->_window);

		this->displayDynamicContents();

		this->_window->draw(this->_time);
		this->_window->display();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		{
			auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (time / 1000000000 > NETWORK_TIMER)
			{
				try
				{
					this->_client->handleNetwork();
				}
				catch (std::runtime_error &)
				{
					start = std::chrono::high_resolution_clock::now();
				}
			}
			auto timing = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
			std::this_thread::sleep_for(std::chrono::microseconds(CYCLE_TIME_MENU - timing));
		}
	}
}

void	SFMLbaseEditor::displayDynamicContents()
{

}

void	SFMLbaseEditor::updateDynamicContents(const sf::Event &)
{

}
