#include "HomingShot.h"
#include "Enemy.h"
#include "Tower.h"

#include <memory>

HomingShot::HomingShot(const Tower& tower, Enemy *enemy_to_shoot) 
    : Shot(tower)
    , mEnemy_to_follow_is_dead(false)
{
    mEnemy_to_shoot = enemy_to_shoot;
    set_target(enemy_to_shoot->get_position());
    enemy_to_shoot->add_following_shot(this);
}

bool HomingShot::follow()
{
    return Shot::follow(mTarget);
}

Enemy *HomingShot::get_enemy_to_shoot() const
{
    return mEnemy_to_shoot;
}

void HomingShot::on_tick()
{
    if(!mEnemy_to_follow_is_dead) set_target(mEnemy_to_shoot->get_position());
    if (Shot::follow(mTarget))
    {
        if (!mEnemy_to_follow_is_dead && mExplosive_radius == 0)
        {
            mEnemy_to_shoot->take_damage(mDamage);
        }
        else
        {
            damaged_an_enemy();
        }
        // continue shooting to where the enemy was last?
        delete this;
    }
}

void HomingShot::set_enemy_to_follow_is_dead(const bool v)
{
    mEnemy_to_follow_is_dead = v;
}
