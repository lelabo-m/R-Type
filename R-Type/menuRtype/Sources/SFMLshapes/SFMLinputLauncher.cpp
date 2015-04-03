#include "SFMLinputLauncher.h"
#include "SFMLbasicText.h"
#include "databaseSFML.h"

SFMLinputLauncher::SFMLinputLauncher(const sf::Vector2<int> &pos, int lengthMax)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_maxMessage = lengthMax;
	this->_background.setTexture(db->getTexture("popupInputBackground"));
	this->_background.setPosition((float)pos.x, (float)pos.y);
	this->_message = SFMLbasicText("", pos, 20);
	this->_area = sf::Rect<int>(pos.x, pos.y, (int)_background.getLocalBounds().width, (int)_background.getLocalBounds().height);

	this->InitInputLabel();
	this->_funcFocus	= std::bind(&SFMLinputLauncher::hoverFunc, this, this);
	this->_funcUnfocus	= std::bind(&SFMLinputLauncher::unhoverFunc, this, this);
}


SFMLinputLauncher::~SFMLinputLauncher()
{
}

void	SFMLinputLauncher::hoverFunc(SFMLshape *)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("popupInputBackgroundHover"));
}

void	SFMLinputLauncher::unhoverFunc(SFMLshape *)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_background.setTexture(db->getTexture("popupInputBackground"));
}
