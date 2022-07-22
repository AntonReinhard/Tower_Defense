#include "MouseItem.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "IndustrialBuilding.h"
#include "MouseHandler.h"
#include "SDL_setup.h"

MouseItem::MouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, LAYERS click_layer) : Clickable(click_layer), Renderable(click_layer)
{
    mName_of_object = name_of_object;
    mClickable_space.x = 0;
    mClickable_space.y = 0;
    mClickable_space.w = 1280;
    mClickable_space.h = 1050;
    mSprite = sprite;
    mLevel = level;
    mGrid_sprite_path = std::string(gConfig_file->value("grid", "path"));
    mGrid_sprite = gTextures->get_texture(mGrid_sprite_path);
}

void MouseItem::render()
{
    SDL_Rect dest;
    int mouse_x;
    int mouse_y;
    //get coordinates
    gMouse_handler->get_mouse_position(&mouse_x, &mouse_y);
    auto grid_offset_x = 0;
    auto grid_offset_y = 0;
    if(mouse_x < 1280)
    {
        grid_offset_x = mouse_x % TILE_WIDTH;
        grid_offset_y = mouse_y % TILE_HEIGHT;
    }

    dest.x = mouse_x - grid_offset_x;
    dest.y = mouse_y - grid_offset_y;
    dest.w = mSprite->get_width();
    dest.h = mSprite->get_height();

    gLayer_handler->render_to_layer(this->mSprite, mClick_layer, {}, dest);
    
    dest.x = 0;
    dest.y = 0;
    dest.w = 1280;
    dest.h = 1024;
    gLayer_handler->render_to_layer(this->mGrid_sprite, LAYERS::BUILDINGOVERLAY, {}, dest);
}


std::string MouseItem::get_name_of_object() const
{
    return this->mName_of_object;
}

void MouseItem::set_name_of_object(const std::string& name)
{
    mName_of_object = name;
}

void MouseItem::on_click(const int mouse_x, const int mouse_y)
{

}

void MouseItem::on_right_click(int mouse_x, int mouse_y)
{
    gMouse_handler->set_item_on_mouse(nullptr);
}

void MouseItem::on_mouse_over(const int mouse_x, const int mouse_y)
{

}
