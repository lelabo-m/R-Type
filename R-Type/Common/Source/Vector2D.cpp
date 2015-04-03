#include <memory>
#include <cstdlib>
#include "Vector2D.h"

Vector2D::Vector2D() : _x(0), _y(0) {}
Vector2D::Vector2D(int x, int y) : _x(x), _y(y) {}
Vector2D::Vector2D(const Vector2D& begin, const Vector2D& end) : _x(end._x - begin._x), _y(end._x - begin._y) {}
Vector2D::~Vector2D() {}
Vector2D::Vector2D(const Vector2D& other) : _x(other._x), _y(other._y) {}
Vector2D::Vector2D(const Vector2D&& other) : _x(std::move(other._x)), _y(std::move(other._y)) {}

Vector2D&	Vector2D::operator=(const Vector2D& other)
{
	_x = other._x;
	_y = other._y;
	return *this;
}

Vector2D&	Vector2D::operator=(const Vector2D&& other)
{
	_x = std::move(other._x);
	_y = std::move(other._y);
	return *this;
}

int		Vector2D::X() const
{
	return _x;
}

int		Vector2D::Y() const
{
	return _y;
}

int&		Vector2D::X()
{
	return _x;
}

int&		Vector2D::Y()
{
	return _y;
}

Vector2D&	Vector2D::operator+(const Vector2D& other)
{
	_x += other._x;
	_y += other._y;
	return *this;
}

Vector2D&	Vector2D::operator-(const Vector2D& other)
{
	_x -= other._x;
	_y -= other._y;
	return *this;
}

Vector2D&	Vector2D::operator*(const Vector2D& other)
{
	_x *= other._x;
	_y *= other._y;
	return *this;
}

void		Vector2D::direction(const Vector2D& begin, const Vector2D& end)
{
	_x = end._x - begin._x;
	_y = end._y - begin._y;
}

void		Vector2D::randomized()
{
	_x = std::rand();
	_y = std::rand();
}

void		Vector2D::normalized(int maxx, int maxy)
{
	_x = _x % maxx;
	_y = _y % maxy;
}

void		Vector2D::limit(int minx, int miny, int maxx, int maxy)
{
	(_x > maxx) ? (_x = maxx) : (_x < minx) ? (_x = minx) : (_x);
	(_y > maxy) ? (_y = maxy) : (_y < miny) ? (_y = miny) : (_y);
}

int		Vector2D::distance(const Vector2D& other) const
{
	return (int)std::sqrt(((other._x - _x) * (other._x - _x))
		+ ((other._y - _y) * (other._y - _y)));
}

void		Vector2D::reverse()
{
	int	tmp = _x;
	_x = _y;
	_y = tmp;
}