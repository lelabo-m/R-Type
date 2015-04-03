#include "databaseSFML.h"

databaseSFML *databaseSFML::inst = NULL;

databaseSFML::databaseSFML()
{
}


databaseSFML::~databaseSFML()
{
	this->_imgDB.clear();
	this->_fontDB.clear();
}

databaseSFML *databaseSFML::getDatabase()
{
	if (inst == NULL)
		inst = new databaseSFML;
	return inst;
}

void databaseSFML::deleteDatabase()
{
	if (inst)
		delete inst;
	inst = NULL;
}

void	databaseSFML::addTexture(const std::string &index, const sf::Texture &texture)
{
	this->_imgDB[index] = texture;
}

void	databaseSFML::addFont(const std::string &index, const sf::Font &font)
{
	this->_fontDB[index] = font;
}

const sf::Texture	&databaseSFML::getTexture(const std::string &index) const
{
	try
	{
		return this->_imgDB.at(index);
	}
	catch (const std::out_of_range &)
	{
		std::string errMessage = "No texture found at index '" + index + "'";
		throw DatabaseException(errMessage.c_str());
	}
}

const sf::Font		&databaseSFML::getFont(const std::string &index) const
{
	try
	{
		return this->_fontDB.at(index);
	}
	catch (const std::out_of_range &)
	{
		std::string errMessage = "No font found at index '" + index + "'";
		throw DatabaseException(errMessage.c_str());
	}
}
