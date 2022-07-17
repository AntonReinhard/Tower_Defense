#include "StaticTargetTower.h"
#include "StaticTargetShot.h"
#include "ConfigFile.h"

StaticTargetTower::StaticTargetTower(const std::string& tower_name, const SDL_Point coords, Level* level, LAYERS const click_layer, LAYERS const render_layer) : Tower(tower_name, coords, level, click_layer, render_layer)
{

}

void StaticTargetTower::create_shot(Enemy& enemy)
{
    new StaticTargetShot(*this, &enemy);
}

void StaticTargetTower::on_tick()
{
    if (mBuilding_window != nullptr) update_building_window();
    Tower::on_tick();
}

void StaticTargetTower::update_building_window()
{
    Tower::update_building_window();
}

void StaticTargetTower::set_stat_strings_for_upgrade_buttons(UpgradeButton& button)
{
    Tower::set_stat_strings_for_upgrade_buttons(button);
}

bool StaticTargetTower::upgrade(const std::string& tower_upgrade_section)
{
    return Tower::upgrade(tower_upgrade_section);
}