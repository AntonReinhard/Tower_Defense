#pragma once
#include "Button.h"
#include "WindowButton.h"

class BuildingWindow;

class UpgradeButton : public WindowButton
{
public:
    UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, const std::string& building_name,
                  const std::string& upgrade_section, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id = 0);
    ~UpgradeButton();

    std::string get_upgrade_section() const;

    void on_mouse_over(int mouse_x, int mouse_y) override;

    void on_mouse_out(int mouse_x, int mouse_y) override;
    void on_click(int mouse_x, int mouse_y) override;

private:
    std::string mUpgrade_section;
    Window* mUpgrade_window;

};
