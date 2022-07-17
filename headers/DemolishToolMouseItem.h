#pragma once
#include "MouseItem.h"

class DemolishToolMouseItem : public MouseItem
{
public:
    DemolishToolMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, LAYERS click_layer);

    void on_click(int mouse_x, int mouse_y) override;

    void on_mouse_over(int mouse_x, int mouse_y) override;

    static void destroy_building(Building* building);
};
