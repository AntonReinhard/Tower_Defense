#include "Button.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "ButtonObject.h"

Button::Button(const std::string& button_name, const SDL_Rect& dim, ButtonObject* obj,
               const LAYERS click_layer, const LAYERS render_layer, const int button_id) 
    : Clickable(click_layer)
    , Renderable(render_layer)
    , mClips{}
    , mButton_sprite{}
    , mObject_to_notify(obj)
    , mButton_id{button_id}
    , mButton_dimensions(dim)
{
    const auto section = "button/" + button_name;
    
    this->mButton_sprite = gTextures->get_texture(gConfig_file->value(section, "path"));

    //initialize the clips
    const int clip_width = gConfig_file->value(section, "clip_width");
    const int clip_height = gConfig_file->value(section, "clip_height");
    for (auto i = 0; i < L_CLICKABLE_STATE::STATES_TOTAL; i++)
    {
        this->mClips[i].x = i*clip_width;
        this->mClips[i].y = 0;
        this->mClips[i].w = clip_width;
        this->mClips[i].h = clip_height;	
    }
    this->set_clickable_space(dim);
}

Button::~Button() = default;

void Button::set_dimension(const SDL_Rect& dim)
{
    this->mButton_dimensions = dim;
}

void Button::add_x_dimension(const int x)
{
    mButton_dimensions.x += x;
}

void Button::add_y_dimension(const int y)
{
    mButton_dimensions.y += y;
}

SDL_Rect Button::get_dimension() const
{
    return mButton_dimensions;
}

void Button::render()
{
    //Show current button sprite
    gLayer_handler->render_to_layer(mButton_sprite, mRender_layer, mClips[this->get_state()], mButton_dimensions);
}

void Button::on_click(int mouse_x, int mouse_y)
{
    this->mObject_to_notify->on_button_press(this->mButton_id, *this);
}
