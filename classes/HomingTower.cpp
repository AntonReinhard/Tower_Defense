#include "HomingTower.h"
#include "HomingShot.h"

HomingTower::HomingTower(const std::string& tower_name, const SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer) 
    : Tower(tower_name, coords, level, click_layer, render_layer)
{
}

void HomingTower::create_shot(Enemy& enemy) 
{
    //These objects take care of themselves
    // ReSharper disable once CppNonReclaimedResourceAcquisition
    new HomingShot(*this, &enemy);
}

void HomingTower::on_tick()
{
    Tower::on_tick();
}

void HomingTower::update_building_window()
{
    Tower::update_building_window();
}
