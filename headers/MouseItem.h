#pragma once
#include "Clickable.h"
#include "LTexture.h"
#include "Resources.h"
#include "Level.h"

class Level;

class MouseItem : public Clickable, public Renderable
{
public:
    MouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, LAYERS click_layer);

    void set_name_of_object(const std::string& name);
    std::string get_name_of_object() const;

    void on_click(int mouse_x, int mouse_y) override;
    void on_right_click(int mouse_x, int mouse_y) override;
    void on_mouse_over(int mouse_x, int mouse_y) override;

    void render() override;

protected:
    std::string mName_of_object;
    LTexture* mSprite;
    Level* mLevel;
    std::string mGrid_sprite_path;
    LTexture* mGrid_sprite;
};
