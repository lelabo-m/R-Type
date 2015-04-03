#include "SFMLanimatedSprite.h"

SFMLanimation::SFMLanimation(const sf::Texture &texure, const std::string &name, const sf::Vector2<size_t> &sizeSprite, size_t index, size_t nbsprites, size_t duration, alignSpriteType alignType)
										: SFMLshape(texure, sf::Rect<int>(0, 0, sizeSprite.x, sizeSprite.y))
{
	this->_name = name;
	this->_nbSprites = nbsprites;
	this->_currentSprite = 0;
	this->_index = index;
	this->_sizeSprite = sizeSprite;
	this->_alignType = alignType;
	this->_loop = 0;
	this->_duration = duration;
	this->_step = 0;
	this->_background.setTextureRect(sf::Rect<int>(
		(sizeSprite.x) * (alignType == VERTICAL),
		(sizeSprite.y * index) * (alignType == HORIZONTAL),
		sizeSprite.x, sizeSprite.y));
}

SFMLanimation::~SFMLanimation()
{
}

size_t	SFMLanimation::Current() const
{
	return this->_currentSprite;
}

size_t	SFMLanimation::Index() const
{
	return this->_index;
}

size_t	SFMLanimation::Size() const
{
	return this->_nbSprites;
}

const sf::Vector2<size_t>		&SFMLanimation::getSizeSprite() const
{
	return this->_sizeSprite;
}

void	SFMLanimation::Play(size_t x, size_t y, sf::RenderWindow& win)
{
	this->_background.setPosition((float)x, (float)y);
	this->_background.setTextureRect(sf::Rect<int>(
		(_sizeSprite.x * _currentSprite) * (_alignType == HORIZONTAL),
		(_sizeSprite.y * _index) * (_alignType == HORIZONTAL),
		_sizeSprite.x, _sizeSprite.y));
	++_step;
	if (_step == _duration)
	{
		(this->_currentSprite < this->_nbSprites) ? (this->_currentSprite += 1)
			: (this->_currentSprite);
		_step = 0;
	}
	win.draw(this->_background);
}

void	SFMLanimation::PlayLoop(size_t x, size_t y, sf::RenderWindow& win)
{
	this->_background.setPosition((float)x, (float)y);
	this->_background.setTextureRect(sf::Rect<int>(
		(_sizeSprite.x * _currentSprite) * (_alignType == HORIZONTAL),
		(_sizeSprite.y * _index) * (_alignType == HORIZONTAL),
		_sizeSprite.x, _sizeSprite.y));
	++_step;
	if (_step == _duration)
	{
		++(this->_currentSprite);
		this->_currentSprite %= this->_nbSprites;
		_step = 0;
	}
	win.draw(this->_background);
}

void	SFMLanimation::Reset()
{
	_currentSprite = 0;
}

void	SFMLanimation::setLoop(bool val)
{
	this->_loop = val;
}

bool	SFMLanimation::getLoop() const
{
	return this->_loop;
}

void	SFMLanimation::Draw(sf::RenderWindow &window)
{
	this->_loop
		? this->PlayLoop(this->_area.left, this->_area.top, window)
		: this->Play(this->_area.left, this->_area.top, window);
}

const std::string		&SFMLanimation::getName() const
{
	return	this->_name;
}

SFMLanimationManager::SFMLanimationManager()
{
}

SFMLanimationManager::~SFMLanimationManager()
{
	_textures.clear();
	_animations.clear();
}

bool		SFMLanimationManager::IsLoad(const std::string& name, size_t id)
{
	if (_textures.find(id) == _textures.end() || (_textures.find(id) != _textures.end() && _textures[id].find(name) == _textures[id].end()))
		return false;
	return true;
}

void		SFMLanimationManager::Load(const std::string& path,
	const std::string& name, size_t id)
{
	_textures[id][name] = sf::Texture();
	_textures[id][name].loadFromFile(path.c_str());
}

void		SFMLanimationManager::Load(const std::string& name, const std::string &objName, size_t id, size_t animnumber, const sf::Vector2<size_t>& sizes, size_t index, size_t nbsriptes, size_t duration, alignSpriteType align)
{
	if (_textures.find(id) != _textures.end() &&
		_textures[id].find(name.c_str()) != _textures[id].end())
		_animations[id][animnumber] = new SFMLanimation(_textures[id][name], objName, sizes, index, nbsriptes, duration, align);
}

SFMLanimation*	SFMLanimationManager::GetAnimation(size_t id, size_t number)
{
	if (_animations.find(id) != _animations.end() &&
		_animations[id].find(number) != _animations[id].end())
		return _animations[id][number];
	return nullptr;
}
