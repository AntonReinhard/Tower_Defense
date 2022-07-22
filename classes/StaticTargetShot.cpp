#include <SDL.h>
#include "EntityHandler.h"
#include "StaticTargetShot.h"
#include "Particles.h"
#include "Tower.h"
#include <iostream>

StaticTargetShot::StaticTargetShot(const Tower& tower, Enemy* enemy) : Shot(tower)
{
    //target is set once the shot exists, it wont change
    mTarget = enemy->get_position();
    if(tower.get_predicts())
    {
        Vector enemies_position;
        enemies_position.x = mTarget.x;
        enemies_position.y = mTarget.y;
        Vector towers_position;
        towers_position.x = tower.get_coords().x - tower.get_dimensions().w / 2;
        towers_position.y = tower.get_coords().y - tower.get_dimensions().h / 2;
        auto const enemies_predicted_position = enemies_position + enemy->get_move_diretion() * enemy->get_move_speed()
        * ((enemies_position - towers_position).abs() / tower.get_projectile_speed());
        mTarget.x = enemies_predicted_position.x;
        mTarget.y = enemies_predicted_position.y;
    }
}

void StaticTargetShot::on_tick()
{
    if (follow())
    {
        damaged_an_enemy();
        delete this;
    }
}

bool StaticTargetShot::follow()
{
    const auto target_reached = Shot::follow(mTarget);

    if (target_reached) {
        Vector coordinates, direction;
        coordinates.x = mTarget.x + (rand() % 7 - 3);
        coordinates.y = mTarget.y + (rand() % 7 - 3);
        direction.x = (rand() % 31 - 15) / 40.0;
        direction.y = (rand() % 31 - 15) / 40.0;
        const auto rotation_speed = (rand() % 31 - 15) / 40.f;
        const auto rotation = float(rand() % 360);
        new Particle("explosion", coordinates, direction, rotation, rotation_speed);
    }
    return target_reached;
}
