#pragma once
#include "Enemy.h"
#include "CoordinatesInDouble.h"

class Tower;

class Shot
{
public:
	Shot(Tower* tower);
	~Shot();
	
	void render(SDL_Point target);
	bool follow(SDL_Point target);

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);


protected:
	SDL_Point mCoords;
	CoordinatesInDouble mCoords_in_double;
	double mProjectile_speed;

private:
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;	
};

