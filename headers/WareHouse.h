#pragma once
#include "Building.h"
#include "Menu.h"

class Warehouse final
    : public Building
{
public:
    Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer);

    void on_tick() override;
    void render() override;
    void update_building_window() override;

    BUILDINGTYPE get_building_type() override;
};
