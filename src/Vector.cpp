#include "Vector.h"
#include <math.h>

Vector::Vector()
{
    _x = 0;
    _y = 0;
    _z = 0;
}

Vector::Vector(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
}

Vector::Vector(const Vector &v)
{
    _x = v._x;
    _y = v._y;
    _z = v._z;
}

Vector::~Vector()
{
    //dtor
}

float Vector::Getx() const
{
    return _x;
}

void Vector::Setx(const float value)
{
     _x = value;
}

float Vector::Gety() const
{
    return _y;
}

void Vector::Sety(const float value)
{
    _y = value;
}

float Vector::Getz() const
{
    return _z;
}

void Vector::Setz(const float value)
{
    _z = value;
}

Vector& Vector::operator=(const Vector &v)
{
    _x = v._x;
    _y = v._y;
    _z = v._z;
    return *this;
}

int Vector::operator==(const Vector &v) const
{
    if( _x != v._x)
        return 0;
    if( _y != v._y)
        return 0;
    if( _z != v._z)
        return 0;
    return 1;
}

double Vector::GetNorm()
{
    float value = sqrt(pow(_x,2) + pow(_y,2) + pow(_z,2));
    return value;
}
