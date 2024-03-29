#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "Building.h"

#include <memory>

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, 
    const std::string& building_name, const std::string& upgrade_section, LAYERS click_layer, 
    LAYERS render_layers, Window* window, int button_id)
    : WindowButton(button_name, dim, obj, click_layer, render_layers, window, button_id)
    , mUpgrade_section(upgrade_section)
{
    SDL_Rect dest{ 1730, 804, 190, 220 };
    std::string building_upgrade_section;
    if (upgrade_section == "Damage" || upgrade_section == "Attackspeed" || upgrade_section == "Range")
    {
        building_upgrade_section = "Tower/upgrade" + upgrade_section;
    }
    else
    {
        building_upgrade_section = building_name + "/upgrade" + upgrade_section;
    }
    mUpgrade_window = new Window(dest, WINDOWS, WINDOWS);

    Resources upgrade_costs;
    upgrade_costs.read_from_config(building_upgrade_section, "%scosts");

    Resources upgrade_maintenance;
    upgrade_maintenance.read_from_config(building_upgrade_section, "%sMain");

    SDL_Color text_color = { 0,0,0,0 };
    dest = { 20, mWindow->get_dim().h - 180, 0, 0 };
    mUpgrade_window->add_text_to_window("Upgrade costs Main", dest, UPGRADEWINDOWCONTENT, text_color);
    
    for (auto i = 0; i < RESOURCES_TOTAL; ++i)
    {
        dest.y += 20;
        mUpgrade_window->add_text_to_window(Resources::get_name(RESOURCETYPES(i)), dest, UPGRADEWINDOWCONTENT, text_color);
        dest.x += 60;
        mUpgrade_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(upgrade_costs.get_resource(RESOURCETYPES(i)))), dest, UPGRADEWINDOWCONTENT, text_color);
        dest.x += 60;
        mUpgrade_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(upgrade_maintenance.get_resource(RESOURCETYPES(i)))), dest, UPGRADEWINDOWCONTENT, text_color);
        dest.x -= 120;
    }

    mUpgrade_window->set_rendering_enabled(false);
    mUpgrade_window->disable();
}

UpgradeButton::~UpgradeButton()
{
    delete mUpgrade_window;
}

std::string UpgradeButton::get_upgrade_section() const
{
    return mUpgrade_section;
}

void UpgradeButton::on_mouse_over(int mouse_x, int mouse_y)
{
    mUpgrade_window->set_rendering_enabled(true);
    //mUpgrade_window->set_clicked(true);
}

void UpgradeButton::on_mouse_out(int mouse_x, int mouse_y)
{
    mUpgrade_window->set_rendering_enabled(false);
    //mUpgrade_window->set_clicked(false);
}

void UpgradeButton::on_click(const int mouse_x, const int mouse_y)
{
    WindowButton::on_click(mouse_x, mouse_y);
}
