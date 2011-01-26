#include "Vector2d.h"

Vector2d Vector2d::operator+(const Vector2d &v) const
{
    return Vector2d( _x + v._x, _y + v._y );
}

Vector2d Vector2d::operator-(const Vector2d &v) const
{
    return Vector2d( _x - v._x, _y - v._y );
}

Vector2d Vector2d::operator*(float scalar) const
{
    return Vector2d( scalar * _x, scalar * _y );
}

Vector2d Vector2d::operator/(float scalar) const
{
    return Vector2d( _x / scalar, _y / scalar );
}

float Vector2d::operator*(const Vector2d &v) const
{
    return _x * v._x + _y * v._y;
}

Vector2d Vector2d::normalize() const
{
    float mag = magnitude();
    return Vector2d( _x / mag, _y / mag );
}

Vector2d operator*(float scalar, const Vector2d &v)
{
    return v * scalar;
}

std::string Vector2d::str() const
{
  std::stringstream s;

  s << "(" << _x << "," << _y << ")";

  return s.str();
}
