#pragma once
#include "Shot.h"

class Enemy;

class HomingShot : 
    public Shot
{
public:
    HomingShot(const Tower& tower, Enemy* enemy_to_shoot);

    Enemy *get_enemy_to_shoot() const;

    void on_tick() override;

    bool follow(); //shot follows an enemy, returns true when hit

    void set_enemy_to_follow_is_dead(bool v);

private:
    Enemy* mEnemy_to_shoot;

    bool mEnemy_to_follow_is_dead;
};
