#include "SFMLplayermulti.h"
#include "databaseSFML.h"

SFMLplayermulti::SFMLplayermulti(const sf::Vector2<int> &pos, const std::string &pseudo, const sf::Texture &texture, playerStatus status) : SFMLshape(texture, pos)
{
	databaseSFML	*db = databaseSFML::getDatabase();

	this->_pseudo = SFMLbasicText(pseudo, sf::Vector2<int>(pos.x + 40, pos.y + 5), 15);
	this->_action = nullptr;

	if (status == HOSTER)
	{
		this->_action = new SFMLbutton("", db->getTexture("hoster"), sf::Vector2<int>(pos.x + 3, pos.y + 3));
	}
}

SFMLplayermulti::~SFMLplayermulti()
{
	if (this->_action != nullptr)
		delete this->_action;
}

void	SFMLplayermulti::Draw(sf::RenderWindow &window)
{
	window.draw(_background);
	window.draw(_pseudo);
	if (_action)
		_action->Draw(window);
}

void SFMLplayermulti::setPosition(const sf::Rect<int> &area)
{
	this->_area = area;
	this->_background.setPosition(static_cast<float>(area.left), static_cast<float>(area.top));
	this->_pseudo.setPosition(static_cast<float>(area.left + 40), static_cast<float>(area.top + 5));
	if (_action)
		this->_action->_background.setPosition(static_cast<float>(area.left + 3), static_cast<float>(area.top + 3));

}
