#ifndef VECTOR2D_H_
# define VECTOR2D_H_

#include <iostream>
#include <cmath>

class Vector2D
{
public:
	Vector2D();
	Vector2D(int, int);
	Vector2D(const Vector2D&, const Vector2D&);
	Vector2D(const Vector2D&);
	Vector2D(const Vector2D&&);
	~Vector2D();
	Vector2D&		operator=(const Vector2D&);
	Vector2D&		operator=(const Vector2D&&);
	Vector2D&		operator+(const Vector2D&);
	Vector2D&		operator-(const Vector2D&);
	Vector2D&		operator*(const Vector2D&);
	void			direction(const Vector2D&, const Vector2D&);
	void			randomized();
	void			normalized(int, int);
	void			limit(int, int, int, int);
	int				distance(const Vector2D&) const;
	void			reverse();
	int				X() const;
	int				Y() const;
	int&			X();
	int&			Y();
private:
	int		_x;
	int		_y;
};

inline std::ostream&	operator<<(std::ostream& out, const Vector2D& v)
{
	out << " Vector : X = " << v.X() << " Y = " << v.Y();
	return out;
}

#endif /* !VECTOR2D_H_ */
