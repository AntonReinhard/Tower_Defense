#pragma once
#include <memory>

#include "MenuItem.h"
#include "Resources.h"

class BuildingMenuItem : public MenuItem
{
public:
    BuildingMenuItem(const std::string& name_of_object, Level *level, SDL_Point coords, LAYERS click_layer, LAYERS render_layer);
    ~BuildingMenuItem();

    void render() override;

    void on_click(int mouse_x, int mouse_y) override;

    std::shared_ptr<Window> create_window();
    
    Resources get_construction_costs() const;

private:
    Resources mConstruction_costs;
};
