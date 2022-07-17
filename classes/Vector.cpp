#include <SDL.h>
#include "Vector.h"

Vector::Vector()
{
    x = 0;
    y = 0;
}

Vector::Vector(const SDL_Point p)
{
    this->x = p.x;
    this->y = p.y;
}

Vector Vector::operator+(const Vector summand) const
{
    Vector result;
    result.x = this->x + summand.x;
    result.y = this->y + summand.y;
    return result;
}

Vector Vector::operator-(const Vector subtrahend) const
{
    Vector result;
    result.x = x - subtrahend.x;
    result.y = y - subtrahend.y;
    return result;
}

Vector Vector::operator*(const Vector factor) const
{
    Vector result;
    result.x = x * factor.x;
    result.y = y * factor.y;
    return result;
}

Vector Vector::operator*(const double v) const
{
    Vector result;
    result.x = x * v;
    result.y = y * v;
    return result;
}

Vector Vector::operator/(const double v) const
{
    Vector result;
    result.x = x / v;
    result.y = y / v;
    return result;
}


double Vector::abs() const
{
    return sqrt(x * x + y * y);
}

Vector Vector::root() const
{
    Vector result;
    result.x = sqrt(x);
    result.y = sqrt(y);
    return result;
}

Vector::operator SDL_Point() const
{
    SDL_Point p;
    p.x = int(this->x);
    p.y = int(this->y);
    return p;
}
