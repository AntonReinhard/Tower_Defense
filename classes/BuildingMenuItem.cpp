#include "BuildingMenuItem.h"
#include "ConfigFile.h"
#include "MouseHandler.h"
#include "BuildingMouseItem.h"
#include "Menu.h"
#include <strstream>

BuildingMenuItem::BuildingMenuItem(const std::string& name_of_object, Level* level, const SDL_Point coords, LAYERS click_layer, LAYERS render_layer)
    : MenuItem(name_of_object, level, coords, click_layer, render_layer)
{
    const auto building_stats_section = mName_of_object + "/stats";
    mConstruction_costs.read_from_config(building_stats_section, "%scosts");
}

BuildingMenuItem::~BuildingMenuItem() = default;

void BuildingMenuItem::render()
{
    MenuItem::render();
}

void BuildingMenuItem::on_click(int mouse_x, int mouse_y)
{
    //	Building::get_default_stats(mName_of_object);
    gMouse_handler->set_item_on_mouse(new BuildingMouseItem(mName_of_object, mSprite, mLevel, mConstruction_costs, LAYERS::OVERLAY));
    mLevel->get_menu()->set_menu_item_window(create_window());
}

std::shared_ptr<Window> BuildingMenuItem::create_window()
{
    std::shared_ptr<Window> menu_item_window;

    //set the construction costs of the building
    const auto building_stats_section = mName_of_object + "/stats";

    Resources storage_limit;
    storage_limit.read_from_config(building_stats_section, "%sLimit");

    Resources maintenance;
    maintenance.read_from_config(building_stats_section, "%sMain");

    const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
    SDL_Color text_color = { 0, 0, 0 ,0 };
    menu_item_window = std::make_shared<Window>(SDL_Rect{ 1280, 364, 400, 220 }, WINDOWS, WINDOWS);

    SDL_Rect rect{ 20, 20, 0, 0 };

    if (kind_of_object == "path")
    {
        auto speed_multiplier = gConfig_file->value(building_stats_section, "speed_multiplier");
        menu_item_window->add_text_to_window("Speed Multi", rect, WINDOWCONTENT, text_color);
        rect.x += 120;
        menu_item_window->add_text_to_window(std::to_string(speed_multiplier), rect, WINDOWCONTENT, text_color);
    }
    else
    {
        menu_item_window->add_text_to_window(mName_of_object, rect, WINDOWCONTENT, text_color);
        rect.y += 20;
        menu_item_window->add_text_to_window("       MaxStorage   Cost(M)", rect, WINDOWCONTENT, text_color);

        rect.x += 10;
        for (auto i = 0; i < RESOURCES_TOTAL; ++i)
        {
            rect.y += 20;
            menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(storage_limit.get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color);
            rect.x += 40;
            menu_item_window->add_text_to_window(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color);
            rect.x += 70;
            menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(mConstruction_costs.get_resource(RESOURCETYPES(i))))+ "(" +Text::remove_trailing_zeros(std::to_string(maintenance.get_resource(RESOURCETYPES(i)))) + ")", rect, WINDOWCONTENT, text_color);
            rect.x -= 110;
        }
        rect = { 220, 40, 0, 0 };

        if (kind_of_object == "onhittower" || kind_of_object == "aoetower" || kind_of_object == "statictargettower")
        {
            const Damage damage{ gConfig_file->value_or_zero(building_stats_section, "phys"),
                gConfig_file->value_or_zero(building_stats_section, "magic"),
                gConfig_file->value_or_zero(building_stats_section, "fire"),
                gConfig_file->value_or_zero(building_stats_section, "water"),
                gConfig_file->value_or_zero(building_stats_section, "elec") };
            const auto range = gConfig_file->value(building_stats_section, "range");
            const auto attack_speed = gConfig_file->value(building_stats_section, "attackspeed");

            menu_item_window->add_text_to_window("Dmg: ", rect, WINDOWS, text_color);
            rect.y += 30;
            menu_item_window->add_text_to_window("AS: ", rect, WINDOWS, text_color);
            rect.y += 30;
            menu_item_window->add_text_to_window("Range: ", rect, WINDOWS, text_color);
            rect.y += 30;
            menu_item_window->add_text_to_window("Damage dist: ", rect, WINDOWS, text_color);
            rect.y += 30;
            //damage distribution text
            menu_item_window->add_text_to_window(
                
                      "P: " + Text::remove_trailing_zeros(std::to_string(damage.get_phys_dmg()))
                    + " M: " + Text::remove_trailing_zeros(std::to_string(damage.get_magic_dmg()))
                    + " F: " + Text::remove_trailing_zeros(std::to_string(damage.get_fire_dmg()))
                    + " W: " + Text::remove_trailing_zeros(std::to_string(damage.get_water_dmg()))
                    + " E: " + Text::remove_trailing_zeros(std::to_string(damage.get_elec_dmg())), rect, WINDOWS, text_color);
            rect.x = menu_item_window->get_dim().x + 280;
            rect.y = menu_item_window->get_dim().y + 40;

            menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(damage.get_dmg_sum())), rect, WINDOWS, text_color);
            rect.y += 30;
            menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(attack_speed)), rect, WINDOWS, text_color);
            rect.y += 30;
            menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(range)), rect, WINDOWS, text_color);

            if (kind_of_object == "aoetower")
            {
                const auto explosive_radius = gConfig_file->value(building_stats_section, "explosiveradius");
                rect.x = menu_item_window->get_dim().x + 220;
                rect.y = menu_item_window->get_dim().y + 160;

                menu_item_window->add_text_to_window("Explosive Radius: ", rect, WINDOWCONTENT, text_color);
                rect.x += 130;
                menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(explosive_radius)), rect, WINDOWCONTENT, text_color);
            }
        }
        else if (kind_of_object == "industrialbuilding")
        {
            Resources produce;
            produce.read_from_config(building_stats_section, "%sproduction");

            menu_item_window->add_text_to_window("Production", rect, WINDOWCONTENT, text_color);

            for (auto i = 0; i < RESOURCES_TOTAL; ++i)
            {
                rect.y += 20;
                menu_item_window->add_text_to_window(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color);
                rect.x += 60;
                menu_item_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(produce.get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color);
                rect.x -= 60;
            }
        }
    }

    return menu_item_window;
}

Resources BuildingMenuItem::get_construction_costs() const
{
    return mConstruction_costs;
}
