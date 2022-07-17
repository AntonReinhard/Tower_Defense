#include "WareHouse.h"
#include "Menu.h"
#include "Production.h"

Warehouse::Warehouse(std::string logistics_building_name, const SDL_Point coords, Level *level, const LAYERS click_layer, const LAYERS render_layer)
    : Building{std::move(logistics_building_name), coords, level, click_layer, render_layer}
{
    //since this is a warehouse it needs to "consume" everything so it requests everything from producing buildings
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        mProducing[RESOURCETYPES(i)] = CONSUMING;
    }
    
    //Warehouse Window is always shown
    Building::create_window();
    mBuilding_window->set_rendering_enabled(true);
}

void Warehouse::on_tick()
{
    update_building_window();
    Building::on_tick();
}

void Warehouse::render()
{
    Building::render();
}

BUILDINGTYPE Warehouse::get_building_type()
{
    return WAREHOUSE;
}

void Warehouse::update_building_window()
{
    Building::update_building_window();
}

