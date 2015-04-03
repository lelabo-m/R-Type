#pragma once

#include <SFML/Graphics.hpp>

class windowSFML
{

private:
	static sf::RenderWindow	*inst;

private:
	const windowSFML &operator=(const windowSFML &) { return *this; };
	const windowSFML &operator=(const windowSFML &&) { return *this; };
	windowSFML(const windowSFML &) {};
	windowSFML() {};
	~windowSFML() {};

public:
	static sf::RenderWindow	*getWindow();
	static void				deleteWindow();

};

