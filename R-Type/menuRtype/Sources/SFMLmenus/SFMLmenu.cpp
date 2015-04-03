#include "SFMLmenu.h"
#include "databaseSFML.h"
#include "menuManager.h"
#include "BackgroundAnimation.h"

SFMLmenu::SFMLmenu()
{
	this->_menuName = "BaseMenu";
	this->_controllerManager = ControllerManager::GetInstance();
	this->_menuManager = menuManager::getInst();
	this->_window = windowSFML::getWindow();
	this->_client = ClientManager::getInstance();
	this->initBaseMenu();
}

SFMLmenu::~SFMLmenu()
{
	this->_images.clear();
	this->_texts.clear();
	this->_forms.clear();
}

void	SFMLmenu::initBaseMenu()
{
	databaseSFML	*db = databaseSFML::getDatabase();

	sf::Sprite	background(db->getTexture("backgroundMenu"));
	sf::Sprite  border(db->getTexture("borderMenu"));

	border.setPosition(0.0, 708.0);
	this->_images.push_back(background);
	this->_images.push_back(border);
}

void	SFMLmenu::launch()
{
	sf::Event		 ev;
	BackgroundAnimation		animation(100, 255, 255, 3, LINEAR);

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	while (this->_menuName == _menuManager->getCurrentMenu())
	{
		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

		this->executeRequests();
		if (!this->_images.empty())
		{
			sf::Sprite sp = this->_images.front();
			this->_images.pop_front();
			animation.next();
			sp.setColor(sf::Color(animation.getValue(), animation.getValue(), animation.getValue(), 255));
			this->_images.push_front(sp);
		}
		while (this->_window->pollEvent(ev))
		{
			this->_controllerManager->Update(ev);
			this->_eventManager.execute(ev);
			this->updateDynamicContents(ev);
		}

		this->_window->clear();
		for (const auto &image : this->_images) this->_window->draw(image);
		for (const auto &text : this->_texts)	this->_window->draw(text);
		for (const auto &form : this->_forms)	form->Draw(*this->_window);

		this->displayDynaicContents();
		this->_window->display();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		{
			auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (time / 1000000 > NETWORK_TIMER)
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
			// (CYCLE_TIME_MENU - timing) -> valeur toujours negative et des fois au retour  a un menu precedent
			//  les valeurs sont plus grande que d'habitude (x10 environ), ce qui cause le freeze sur l'appel systeme nanosleep()
			//  ------> nanosleep({4294967295, 0});
			if (timing < CYCLE_TIME_MENU)
				std::this_thread::sleep_for(std::chrono::microseconds(CYCLE_TIME_MENU - timing));
		}
	}
}

void	SFMLmenu::executeRequests()
{
	NetClient *nc = &(_client->getNetClient());

	if (nc->getConnectionState() == NetClientManager::DISCONNECTED)
	{
		this->_menuManager->goTo("Launcher");
	}
}

void SFMLmenu::updateDynamicContents(const sf::Event &)
{

}

void SFMLmenu::displayDynaicContents()
{

}
