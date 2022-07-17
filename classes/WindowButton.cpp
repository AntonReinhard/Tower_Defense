#include "WindowButton.h"
#include "LayerHandler.h"

WindowButton::WindowButton(const std::string& button_name, const SDL_Rect dim, ButtonObject* obj, const LAYERS click_layer, const LAYERS render_layer, Window* window, const int button_id) 
    : Button(button_name, dim, obj, click_layer, render_layer, button_id)
    , mWindow(window)
{
    
}

WindowButton::~WindowButton() = default;

void WindowButton::render()
{
    if(mWindow->is_rendering_enabled())
    {
        enable();
        gLayer_handler->render_to_layer(mButton_sprite, mRender_layer, mClips[this->get_state()], mButton_dimensions);
    }
    else
    {
        disable();
    }
}

void WindowButton::on_click(int mouse_x, int mouse_y)
{
    mWindow->set_rendering_enabled(true);
    mWindow->enable();
    mWindow->set_clicked(true);
    Button::on_click(mouse_x, mouse_y);
}

Window* WindowButton::get_window()
{
    return mWindow;
}
