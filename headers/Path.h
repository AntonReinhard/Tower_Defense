#pragma once
#include "Building.h"
#include "Menu.h"
#include "Constants.h"

class Path final
    : public Building
{
public:
    Path(const std::string& path_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

    BUILDINGTYPE get_building_type() override;

    double get_speed_multiplier() const;

private:

    //how fast a unit is compared to normal on this road
    double mSpeed_multiplier;
};