#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <sstream>

class Vector2d 
{
  public:
    Vector2d(float _x=0.0, float _y=0.0) : x(_x), y(_y) { }
    Vector2d( const Vector2d &v ) : x(v.x), y(v.y) { }
    ~Vector2d() { }

    float magnitude() const { return sqrt(x*x + y*y); }
    float getX() const { return x; }
    float getY() const { return y; }

    void setX( float val ) { x = val; }
    void setY( float val ) { y = val; }

    Vector2d operator+(const Vector2d &v) const;
    Vector2d operator-(const Vector2d &v) const;
    Vector2d operator*(float scalar) const;
    Vector2d operator/(float scalar) const;
    float operator*(const Vector2d &v) const;
    Vector2d normalize() const;
    std::string str() { std::stringstream s;
    s << "(" << x << "," << y << ")";
    return s.str();
    }

  private:
    float x, y;
};

Vector2d operator*(float scalar, const Vector2d &v);

#endif /* VECTOR2D_H */
