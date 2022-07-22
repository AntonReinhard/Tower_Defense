#pragma once
#include "Tower.h"

class OnHitTower final : public Tower
{
public:
    OnHitTower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

    void create_shot(Enemy& enemy) override;
    void update_building_window() override;
    void on_tick() override;
    void set_stat_strings_for_upgrade_buttons(UpgradeButton& button) override;
    bool upgrade(const std::string& building_upgrade_section) override;
};
