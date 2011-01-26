#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <sstream>

class Vector2d 
{
  public:
    Vector2d(float xVal=0.0, float yVal=0.0) : _x(xVal), _y(yVal) { }
    Vector2d( const Vector2d &v ) : _x(v._x), _y(v._y) { }
    ~Vector2d() { }

    float magnitude() const { return sqrt(_x*_x + _y*_y); }
    float x() const { return _x; }
    float y() const { return _y; }

    void setX( float val ) { _x = val; }
    void setY( float val ) { _y = val; }

    Vector2d operator+(const Vector2d &v) const;
    Vector2d operator-(const Vector2d &v) const;
    Vector2d operator*(float scalar) const;
    Vector2d operator/(float scalar) const;
    float operator*(const Vector2d &v) const;
    Vector2d normalize() const;

    std::string str() const;

  private:
    float _x, _y;
};

Vector2d operator*(float scalar, const Vector2d &v);

#endif /* VECTOR2D_H */
