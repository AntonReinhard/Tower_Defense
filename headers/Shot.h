#pragma once
#include "LTexture.h"
#include "Vector.h"
#include "Damage.h"
#include "Entity.h"

class Tower;

class Shot
    : public Entity
{
public:
    explicit Shot(const Tower& tower);
    
    void render() override; //renders the shot
    void on_tick() override = 0;
    virtual bool damaged_an_enemy(); // shot tries to damage an enemy, if successful return true, otherwise false
    void points_projectile_to_target(SDL_Rect& dest, SDL_Point& center, double& angle) const;
    bool follow(SDL_Point target); //follows the target and returns true when it hits

    ENTITYTYPE get_type() override;

    SDL_Point get_coords() const;
    void set_coords(SDL_Point coords);

    void set_target(SDL_Point target);

protected:
    SDL_Point mCoords;
    Vector mCoords_in_double;
    double mProjectile_speed;
    double mExplosive_radius;

    //the damage this shot will do
    Damage mDamage;
    SDL_Point mTarget; //the point the shot is targeting

private:
    LTexture *mSprite; //Texture
    SDL_Rect mSprite_dimensions; //size of texture	
};

