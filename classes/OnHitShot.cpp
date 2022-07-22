#include "OnHitShot.h"
#include "Tower.h"
#include "EntityHandler.h"

OnHitShot::OnHitShot(const Tower& tower, Enemy* enemy) 
    : Shot(tower)
{
    auto location_to_shoot = enemy->get_position();
    auto const t_x = float(tower.get_coords().x + tower.get_dimensions().w / 2);
    auto const t_y = float(tower.get_coords().y + tower.get_dimensions().h / 2);
    auto const l_x = float(location_to_shoot.x);
    auto const l_y = float(location_to_shoot.y);
    auto m = (t_y - l_y) / (t_x - l_x);
    // can not divide by zero
    // for the moment set some big numbers for m if this happens
    if(t_x - l_x == 0)
    {
        if (t_y - l_y < 0) m = 1000000000000; 
        else m = -1000000000000;
    }
    auto const n = t_y - m * t_x;
    
    //TODO: the fuck are all these magic numbers?!
    float y = 0;
    float x = 0;
    //enemy is right of the tower
    if(l_x > t_x)
    {
        y = m * 1280 + n;
        if(y >= 0 && y <= 1024)
        {
            location_to_shoot.x = 1280;
            location_to_shoot.y = y;
        }
        else if(l_y > t_y)
        {
            x = (1024 - n) / m;
            if(x <= 1280 && x >= 0)
            {
                location_to_shoot.x = x;
                location_to_shoot.y = 1024;
            }
        }
        else
        {
            x = (0 - n) / m;
            location_to_shoot.x = x;
            location_to_shoot.y = 0;
        }
    }
    //enemy is left of the tower
    else
    {
        y = m * 0 + n;
        if (y >= 0 && y <= 1024)
        {
            location_to_shoot.x = 0;
            location_to_shoot.y = y;
        }
        else if (l_y > t_y)
        {
            x = (1024 - n) / m;
            if (x <= 1280 && x >= 0)
            {
                location_to_shoot.x = x;
                location_to_shoot.y = 1024;
            }
        }
        else
        {
            x = (0 - n) / m;
            location_to_shoot.x = x;
            location_to_shoot.y = 0;
        }
    }
    mTarget = location_to_shoot;
}

void OnHitShot::on_tick()
{
    if (damaged_an_enemy()) delete this;
    if (Shot::follow(mTarget)) delete this;
}

bool OnHitShot::follow()
{
    return Shot::follow(mTarget);
}


bool OnHitShot::damaged_an_enemy()
{
    const auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
    auto damaged_an_enemy = false;
    //first iterate through all enemies and look if the shot itself(without explosionradius) hit something
    for (auto it : *all_enemies)
    {
        if (Tower::enemy_in_range(dynamic_cast<Enemy&>(*it), 0, get_coords()))
        {
            //if the shot hit something damage all enemies that are in the explosion radius
            for (auto it1 : *all_enemies)
            {
                if (Tower::enemy_in_range(dynamic_cast<Enemy&>(*it1), mExplosive_radius, get_coords()))
                {
                    //TODO: more damage closer to impact?
                    dynamic_cast<Enemy&>(*it1).take_damage(mDamage);
                    damaged_an_enemy = true;
                    if (mExplosive_radius == 0) return damaged_an_enemy;
                }
            }
            break;
        }
    }
    return damaged_an_enemy;
}
