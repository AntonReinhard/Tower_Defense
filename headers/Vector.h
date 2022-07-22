#pragma once
#include <SDL.h>

class Vector
{
public:
    Vector();
    explicit Vector(SDL_Point p);
    
    double x;
    double y;

    Vector operator+(Vector) const;
    Vector operator-(Vector) const;
    Vector operator*(Vector) const;
    Vector operator*(double) const;
    Vector operator/(double) const;
    Vector root() const;
    double abs() const;
    operator SDL_Point() const;
};
