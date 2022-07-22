#pragma once
#include "MouseItem.h"

class BuildingMouseItem : public MouseItem
{
public:
    BuildingMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const Resources& construction_costs, LAYERS click_layer);

    void on_click(int mouse_x, int mouse_y) override;
    void on_mouse_over(int mouse_x, int mouse_y) override;

    void render() override;

private:
    Resources mConstruction_costs;
    TILETYPES mTile_to_build_on;
};
