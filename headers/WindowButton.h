#pragma once
#include "Button.h"
#include "Window.h"


class WindowButton : public Button
{
public:
    WindowButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, LAYERS click_layer, LAYERS render_layer, Window* window, int button_id = 0);
    ~WindowButton();

    void render() override;

    void on_click(int mouse_x, int mouse_y) override;

    Window* get_window();

private:

protected:
    Window* mWindow; //window where the button is rendered on
};
