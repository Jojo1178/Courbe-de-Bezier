#include "Point.h"

Point::Point()
{
    _x = 0;
    _y = 0;
    _z = 0;
}

Point::Point(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
}

Point::Point(const Point & p)
{
    _x = p._x;
    _y = p._y;
    _z = p._z;
}

Point::~Point()
{
    //dtor
}


float Point::Getx() const
{
    return _x;
}

void Point::Setx(const float value)
{
     _x = value;
}

float Point::Gety() const
{
     return _y;
}

void Point::Sety(const float value)
{
    _y = value;
}

float Point::Getz() const
{
    return _z;
}

void Point::Setz(const float value)
{
    _z = value;
}

Point& Point::operator=(const Point &p)
{
    _x = p._x;
    _y = p._y;
    _z = p._z;
    return *this;
}

int Point::operator==(const Point &p) const
{
    if( _x != p._x)
        return 0;
    if( _y != p._y)
        return 0;
    if( _z != p._z)
        return 0;
    return 1;
}

int Point::operator!=(const Point &p) const
{
    if((_x == p._x) && (_y == p._y) && (_z == p._z))
        return 0;

    return 1;
}

int Point::operator<(const Point &p) const
{
    if( _x == p._x && _y == p._y && _z < p._z)
        return 1;
    if( _x == p._x && _y < p._y)
        return 1;
    if( _x < p._x )
        return 1;

    return 0;
}
