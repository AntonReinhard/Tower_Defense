#include "MenuItem.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "MouseHandler.h"

MenuItem::MenuItem(const std::string& name_of_object, Level *level, const SDL_Point coords, LAYERS click_layer, LAYERS render_layer)
    : Clickable(click_layer), Renderable(render_layer), mCoords(coords), mLevel(level)
{
    mName_of_object = name_of_object;
    mSprite = gTextures->get_texture(gConfig_file->value(name_of_object + "/sprite", "path"));

    mClickable_space.x = mCoords.x;
    mClickable_space.y = mCoords.y;
    mClickable_space.w = mSprite->get_width();
    mClickable_space.h = mSprite->get_height();
}

MenuItem::~MenuItem()
{
}

void MenuItem::render()
{
    gLayer_handler->render_to_layer(mSprite, mRender_layer, {}, mClickable_space);
}

void MenuItem::on_click(int mouse_x, int mouse_y)
{
}

LTexture* MenuItem::get_sprite() const
{
    return this->mSprite;
}

SDL_Point MenuItem::get_coords() const
{
    return this->mCoords;
}
