#include "windowSFML.h"

sf::RenderWindow *windowSFML::inst = NULL;

sf::RenderWindow *windowSFML::getWindow()
{
	if (inst == NULL)
		inst = new sf::RenderWindow(sf::VideoMode(1280, 875, 32), "R-Type", sf::Style::Titlebar | sf::Style::Close);
	return inst;
}

void windowSFML::deleteWindow()
{
	if (inst)
	{
		inst->close();
		delete inst;
	}
	inst = NULL;
}
