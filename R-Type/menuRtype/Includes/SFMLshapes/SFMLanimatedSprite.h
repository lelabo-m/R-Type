#pragma once
#include <list>
#include <map>
#include "SFMLshape.h"

enum alignSpriteType
{
	HORIZONTAL = 0,
	VERTICAL
};

class SFMLanimation :
	public SFMLshape
{
private:
	std::string			_name;
	sf::Vector2<size_t>	_sizeSprite;
	size_t				_nbSprites;
	size_t				_currentSprite;
	size_t				_index;
	alignSpriteType		_alignType;
	size_t				_duration;
	size_t				_step;
	bool				_loop;

public:
	SFMLanimation(const sf::Texture &, const std::string &, const sf::Vector2<size_t> &, size_t, size_t, size_t, alignSpriteType = HORIZONTAL);
	virtual ~SFMLanimation();
public:
	void	Play(size_t x, size_t y, sf::RenderWindow&);
	void	PlayLoop(size_t x, size_t y, sf::RenderWindow&);
	size_t	Current() const;
	size_t	Index() const;
	size_t	Size() const;
	void	Reset();
	void	setLoop(bool);
	bool	getLoop() const;
	const	std::string &getName() const;
	const sf::Vector2<size_t>		&getSizeSprite() const;
	virtual void Draw(sf::RenderWindow &);
};

class SFMLanimationManager
{
public:
	SFMLanimationManager();
	~SFMLanimationManager();
	bool			IsLoad(const std::string& name, size_t id);
	void			Load(const std::string&, const std::string&, size_t);
	void			Load(const std::string&, const std::string&, size_t, size_t, const sf::Vector2<size_t>&, size_t, size_t, size_t, alignSpriteType = HORIZONTAL);
	SFMLanimation	*GetAnimation(size_t, size_t);
private:
	std::map < size_t, std::map<std::string, sf::Texture> >		_textures;
	std::map< size_t, std::map<size_t, SFMLanimation*> >		_animations;
};
