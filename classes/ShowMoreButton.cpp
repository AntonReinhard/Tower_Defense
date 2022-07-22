#include "ShowMoreButton.h"

ShowMoreButton::ShowMoreButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id) 
    : WindowButton(button_name, dim, obj, click_layer, render_layers, window, button_id)
{
    
}

ShowMoreButton::~ShowMoreButton() = default;
