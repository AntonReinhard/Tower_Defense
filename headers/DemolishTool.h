#pragma once
#include "MenuItem.h"

class DemolishTool : public MenuItem
{
public:
    DemolishTool(Level* level, SDL_Point coords);
    ~DemolishTool();

    void render() override;

    void on_click(int mouse_x, int mouse_y) override;
};
