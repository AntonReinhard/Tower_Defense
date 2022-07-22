#include "MouseItem.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "IndustrialBuilding.h"
#include "StaticTargetTower.h"
#include "HomingTower.h"
#include "MouseHandler.h"
#include "SDL_setup.h"
#include "WareHouse.h"
#include "Path.h"
#include "BuildingMouseItem.h"
#include "OnHitTower.h"

BuildingMouseItem::BuildingMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const Resources& construction_costs, LAYERS click_layer) 
    : MouseItem(name_of_object, sprite, level, click_layer)
{
    mConstruction_costs = construction_costs;
    int i = gConfig_file->value(name_of_object + "/stats", "tile");
    mTile_to_build_on = static_cast<TILETYPES>(i);
}

void BuildingMouseItem::render()
{
    MouseItem::render();
}

void BuildingMouseItem::on_click(const int mouse_x, const int mouse_y)
{
    SDL_Point p;
    const auto grid_offset_x = mouse_x % TILE_WIDTH;
    const auto grid_offset_y = mouse_y % TILE_HEIGHT;
    p.x = mouse_x - grid_offset_x;
    p.y = mouse_y - grid_offset_y;
    const auto tile_x = mouse_x / TILE_WIDTH;
    const auto tile_y = mouse_y / TILE_HEIGHT;
    const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");

    int x_size = gConfig_file->value_or_zero(mName_of_object + "/stats", "size_x");
    int y_size = gConfig_file->value_or_zero(mName_of_object + "/stats", "size_y");
    if (x_size <= 0) x_size = 1;
    if (y_size <= 0) y_size = 1;

    auto can_build = true;

    for (auto x_i = 0; x_i < x_size; x_i++) {
        for (auto y_i = 0; y_i < y_size; y_i++)
        {
            if (tile_x + x_i >= MATRIX_WIDTH || tile_y + y_i >= MATRIX_HEIGHT)
            {
                can_build = false;
                break;
            }
            if (mLevel->get_map_matrix(tile_x + x_i, tile_y + y_i) != mTile_to_build_on ||
                mLevel->get_building_matrix(tile_x + x_i, tile_y + y_i) != nullptr) {
                can_build = false;
                break;
            }
        }
    }

    if (can_build && mLevel->get_resources().sub(mConstruction_costs))
    {
        if (kind_of_object == "homingtower") new HomingTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
        if (kind_of_object == "statictargettower") new StaticTargetTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
        if (kind_of_object == "onhittower") new OnHitTower(mName_of_object, p, mLevel, LAYERS::BUILDINGS, LAYERS::BUILDINGS);
        if (kind_of_object == "industrialbuilding") new IndustrialBuilding(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
        if (kind_of_object == "warehouse") new Warehouse(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
        if (kind_of_object == "path") new Path(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
    }
}

void BuildingMouseItem::on_mouse_over(const int mouse_x, const int mouse_y)
{
    if (this->get_state() == MOUSE_DOWN_LEFT)
    {
        SDL_Point p;
        const auto grid_offset_x = mouse_x % TILE_WIDTH;
        const auto grid_offset_y = mouse_y % TILE_HEIGHT;
        p.x = mouse_x - grid_offset_x;
        p.y = mouse_y - grid_offset_y;
        const auto tile_x = mouse_x / TILE_WIDTH;
        const auto tile_y = mouse_y / TILE_HEIGHT;
        const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
        const auto tile_type = mLevel->get_map_matrix(tile_x, tile_y);
        if (tile_type == mTile_to_build_on && mLevel->get_building_matrix(tile_x, tile_y) == nullptr)
        {
            if (kind_of_object == "path")
            {
                if (mLevel->get_resources().sub(mConstruction_costs))
                {
                    new Path(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
                }
            }
        }
    }
}
