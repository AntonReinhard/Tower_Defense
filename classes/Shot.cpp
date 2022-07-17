#include <cstdlib>
#include <cmath>
#include "Shot.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Tower.h"
#include "LayerHandler.h"
#include "Particles.h"
#include "EntityHandler.h"

Shot::Shot(const Tower& tower) 
    : Entity(LAYERS::SHOTS)
    , mCoords()
    , mSprite()
    , mSprite_dimensions()
    , mTarget()
{
    mSprite = gTextures->get_texture(gConfig_file->value(tower.get_projectile_name() + "/sprite", "path"));
    mSprite_dimensions.w = gConfig_file->value(tower.get_projectile_name() + "/sprite", "image_width");
    mSprite_dimensions.h = gConfig_file->value(tower.get_projectile_name() + "/sprite", "image_height");
    mSprite_dimensions.x = 0;
    mSprite_dimensions.y = 0;

    //setting up coordinates
    mCoords = tower.get_coords();
    mProjectile_speed = tower.get_projectile_speed();

    //middle of a shot is its center, not top left (makes the most sense for shots)
    mCoords.x = mCoords.x + tower.get_dimensions().w / 2;
    mCoords.y = mCoords.y + tower.get_dimensions().h / 2;
    mCoords_in_double.x = mCoords.x;
    mCoords_in_double.y = mCoords.y;

    mDamage = tower.get_damage();
    mExplosive_radius = tower.get_explosive_radius();
}

void Shot::render()
{
    SDL_Rect dest;
    SDL_Point center;
    auto angle_in_deg = 0.0;
    const auto flip = SDL_FLIP_NONE;
    this->points_projectile_to_target(dest, center, angle_in_deg);

    gLayer_handler->renderex_to_layer(this->mSprite, mRender_layer, {}, dest, angle_in_deg, center, flip);
}

bool Shot::damaged_an_enemy()
{
    const auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
    auto damaged_an_enemy = false;
    for (auto it : *all_enemies)
    {
        if (Tower::enemy_in_range(dynamic_cast<Enemy&>(*it), mExplosive_radius, get_coords()))
        {
            dynamic_cast<Enemy&>(*it).take_damage(mDamage);
            damaged_an_enemy = true;
            if (mExplosive_radius == 0) return damaged_an_enemy;
        }
    }
    return damaged_an_enemy;
}

void Shot::points_projectile_to_target(SDL_Rect& dest, SDL_Point& center, double& angle) const
{
    double angle_in_rad;
    const double x_d = mTarget.x - mCoords.x;
    const double y_d = mTarget.y - mCoords.y;
    const auto dist_to_enemy = sqrt(x_d * x_d + y_d * y_d);

    if (x_d < 0)
    {
        angle_in_rad = asin(-(y_d / dist_to_enemy));
    }
    else
    {
        if (y_d < 0) angle_in_rad = asin(x_d / dist_to_enemy) + (M_PI / 2);
        else angle_in_rad = asin(-(x_d / dist_to_enemy)) - (M_PI / 2);
    }
    angle = (angle_in_rad / M_PI * 180);

    center.x = mSprite_dimensions.w / 2;
    center.y = mSprite_dimensions.h / 2;
    dest.x = mCoords.x - mSprite_dimensions.w / 2;
    dest.y = mCoords.y - mSprite_dimensions.h / 2;
    dest.w = 0;
    dest.h = 0;
}

bool Shot::follow(const SDL_Point target)
{
    const auto travel_dist = mProjectile_speed / 60.0;
    const double x_d = target.x - mCoords.x;
    const auto x_d_abs = abs(x_d); //take the absolute value for further calculations
    const double y_d = target.y - mCoords.y;
    const auto y_d_abs = abs(y_d); //same as above
    const auto dist_to_enemy = sqrt(x_d * x_d + y_d * y_d);

    if(dist_to_enemy < travel_dist)
    {
        mCoords.x = target.x;
        mCoords.y = target.y;

        return true;
    }
    mCoords_in_double.x += travel_dist * (x_d / (x_d_abs + y_d_abs));
    mCoords_in_double.y += travel_dist * (y_d / (x_d_abs + y_d_abs));
    mCoords.x = static_cast<int>(mCoords_in_double.x);
    mCoords.y = static_cast<int>(mCoords_in_double.y);

    return false;
}

ENTITYTYPE Shot::get_type()
{
    return ENTITYTYPE::SHOT;
}

SDL_Point Shot::get_coords() const
{
    return mCoords;
}

void Shot::set_coords(const SDL_Point coords)
{
    mCoords = coords;
}

void Shot::set_target(const SDL_Point target)
{
    mTarget = target;
}
