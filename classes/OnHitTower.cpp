#include "OnHitTower.h"
#include "OnHitShot.h"

OnHitTower::OnHitTower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer)
    : Tower(tower_name, coords, level, click_layer, render_layer)
{
    
}

void OnHitTower::create_shot(Enemy& enemy)
{
    new OnHitShot(*this, &enemy);
}

void OnHitTower::update_building_window()
{
    Tower::update_building_window();
}

void OnHitTower::on_tick()
{
    Tower::on_tick();
}

void OnHitTower::set_stat_strings_for_upgrade_buttons(UpgradeButton& button)
{
    Tower::set_stat_strings_for_upgrade_buttons(button);
}

bool OnHitTower::upgrade(const std::string& building_upgrade_section)
{
    return Tower::upgrade(building_upgrade_section);
}
