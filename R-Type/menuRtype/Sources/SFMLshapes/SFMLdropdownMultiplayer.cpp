#include <iostream>
#include "SFMLdropdownMultiplayer.h"
#include "databaseSFML.h"

SFMLdropdownMultiplayer::SFMLdropdownMultiplayer(	const sf::Texture &upButton, const sf::Vector2<int> &posUp,
													const sf::Texture &downButton, const sf::Vector2<int> &posDown,
													const sf::Vector2<int> &posObjs,  int nbVisible)
	: SFMLdropdown(upButton, posUp, downButton, posDown, posObjs, nbVisible)
{
	this->initDropdownMulti();
}

SFMLdropdownMultiplayer::~SFMLdropdownMultiplayer()
{
}

void	SFMLdropdownMultiplayer::initDropdownMulti()
{
	_up->_funcFocus = std::bind(&SFMLdropdownMultiplayer::upHover, this, _up);
	_up->_funcUnfocus = std::bind(&SFMLdropdownMultiplayer::upUnhover, this, _up);
	_down->_funcFocus = std::bind(&SFMLdropdownMultiplayer::downHover, this, _down);
	_down->_funcUnfocus = std::bind(&SFMLdropdownMultiplayer::downUnhover, this, _down);

}

void	SFMLdropdownMultiplayer::upHover(SFMLshape *)
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_up->_background.setTexture(db->getTexture("dropdownMultiUpHover"));
}

void	SFMLdropdownMultiplayer::upUnhover(SFMLshape *)
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_up->_background.setTexture(db->getTexture("dropdownMultiUp"));
}

void	SFMLdropdownMultiplayer::downHover(SFMLshape *)
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_down->_background.setTexture(db->getTexture("dropdownMultiDownHover"));
}

void	SFMLdropdownMultiplayer::downUnhover(SFMLshape *)
{
	databaseSFML		*db = databaseSFML::getDatabase();

	_down->_background.setTexture(db->getTexture("dropdownMultiDown"));
}
