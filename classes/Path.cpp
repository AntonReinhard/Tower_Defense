#include "Path.h"
#include "Building.h"
#include "Menu.h"
#include "ConfigFile.h"

Path::Path(const std::string& path_name, const SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer) 
    : Building(path_name, coords, level, click_layer, render_layer)
    , mSpeed_multiplier()
{
    mSpeed_multiplier = gConfig_file->value(path_name + "/stats", "speed_multiplier");
}

BUILDINGTYPE Path::get_building_type()
{
    return STREET;
}

double Path::get_speed_multiplier() const
{
    return this->mSpeed_multiplier;
}
