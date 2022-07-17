#pragma once
#include "Shot.h"

class Enemy;

//shot that flies to a static location and triggers when it reaches the location
class StaticTargetShot final : 
    public Shot
{
public:
    StaticTargetShot(const Tower& tower, Enemy* enemy);

    void on_tick() override;
    
    bool follow(); //shot follows to a location
};
