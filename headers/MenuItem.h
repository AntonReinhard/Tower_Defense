#pragma once
#include "Renderable.h"
#include <string>
#include <SDL.h>
#include "LTexture.h"
#include "Clickable.h"
#include "Window.h"

class Level;

class MenuItem:
    public Clickable, public Renderable
{
public:
    MenuItem(const std::string& name_of_object, Level *level, SDL_Point coords, LAYERS click_layer, LAYERS render_layer);
    ~MenuItem();

    void render() override;

    void on_click(int mouse_x, int mouse_y) override;

    LTexture* get_sprite() const;
    SDL_Point get_coords() const;

protected:
    SDL_Point mCoords; //coordinates, where the item is placed in the menu
    LTexture* mSprite;
    std::string mName_of_object; // for example the tower name
    Level* mLevel;
};
