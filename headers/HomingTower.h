#pragma once
#include "Tower.h"

class HomingTower final :
    public Tower
{
public:
    HomingTower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

    void create_shot(Enemy& enemy) override;
    void on_tick() override;
    void update_building_window() override;
};
