#pragma once
#include "Shot.h"

class Enemy;

class OnHitShot final : public Shot
{
public:
    OnHitShot(const Tower& tower, Enemy* enemy);

    void on_tick() override;

    bool damaged_an_enemy() override;

    bool follow();
};
