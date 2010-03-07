#include "Vector2d.h"

Vector2d Vector2d::operator+(const Vector2d &v) const
{
    return Vector2d( x + v.x, y + v.y );
}

Vector2d Vector2d::operator-(const Vector2d &v) const
{
    return Vector2d( x - v.x, y - v.y );
}

Vector2d Vector2d::operator*(float scalar) const
{
    return Vector2d( scalar * x, scalar * y );
}

Vector2d Vector2d::operator/(float scalar) const
{
    return Vector2d( x / scalar, y / scalar );
}

float Vector2d::operator*(const Vector2d &v) const
{
    return x * v.x + y * v.y;
}

Vector2d Vector2d::normalize() const
{
    float mag = magnitude();
    return Vector2d( x / mag, y / mag );
}

Vector2d operator*(float scalar, const Vector2d &v)
{
    return v * scalar;
}
