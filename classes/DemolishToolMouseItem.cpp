#include "DemolishToolMouseItem.h"
#include "SDL_setup.h"
#include "Building.h"

DemolishToolMouseItem::DemolishToolMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const LAYERS click_layer)
    : MouseItem(name_of_object, sprite, level, click_layer)
{

}

void DemolishToolMouseItem::on_click(const int mouse_x, const int mouse_y)
{
    const auto tile_x = mouse_x / TILE_WIDTH;
    const auto tile_y = mouse_y / TILE_HEIGHT;
    
    destroy_building(mLevel->get_building_matrix(tile_x, tile_y));
}

void DemolishToolMouseItem::on_mouse_over(const int mouse_x, const int mouse_y)
{
    if (this->get_state() == MOUSE_DOWN_LEFT)
    {
        const auto tile_x = mouse_x / TILE_WIDTH;
        const auto tile_y = mouse_y / TILE_HEIGHT;
        destroy_building(mLevel->get_building_matrix(tile_x, tile_y));
    }
}

void DemolishToolMouseItem::destroy_building(Building* building)
{
    if (building != nullptr && building->is_destroyable())
    {
        building->demolish();
        delete building;
    }
}
