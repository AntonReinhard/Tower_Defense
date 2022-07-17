#pragma once
#include "Button.h"
#include "WindowButton.h"

class ShowMoreButton : public WindowButton
{
public:
    ShowMoreButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id);
    ~ShowMoreButton();

private:
};
