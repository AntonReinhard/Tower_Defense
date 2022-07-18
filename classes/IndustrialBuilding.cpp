#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Carriage.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, const SDL_Point coords, Level* level, const LAYERS click_layer, const LAYERS render_layer)
    : Building(std::move(industrial_building_name), coords, level, click_layer, render_layer)
{
    mCarriage = new Carriage("Carriage", mLevel, ENEMIES, this, reinterpret_cast<Building*>(mLevel->get_main_building()));
}

void IndustrialBuilding::update_building_window()
{
    Building::update_building_window();
    for (auto i = 0; i < RESOURCES_TOTAL; ++i)
    {
        mProduction_values[i]->set_text(Text::remove_trailing_zeros(std::to_string(mProduce.get_display_resources().get_resource(RESOURCETYPES(i)))));
    }
}

std::shared_ptr<Window> IndustrialBuilding::create_window()
{
    Building::create_window();

    //add production text in the building window
    const SDL_Color text_color = { 0,0,0,0 };
    SDL_Rect dest{ 0, 0, 200, 40 };
    mBuilding_window->add_text_to_window("        Production", dest, WINDOWCONTENT, text_color);

    for (auto i = 0; i < mProduction_values.size(); ++i)
    {
        dest.y += 20;
        mBuilding_window->add_text_to_window(Resources::get_name(RESOURCETYPES(i)), dest, WINDOWCONTENT, text_color);

        dest.x += 60;
        mProduction_values[i] = mBuilding_window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(mProduce.get_display_resources().get_resource(RESOURCETYPES(i)))), dest, WINDOWCONTENT, text_color);
        dest.x -= 60;
    }

    return mBuilding_window;
}

void IndustrialBuilding::on_tick()
{
    mIdle = !mCurrent_resources.sub_possible(mMaintenance);
    Building::on_tick();
    if (mBuilding_window != nullptr) update_building_window();
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
    return INDUSTRIAL_BUILDING;
}

void IndustrialBuilding::on_click(const int mouse_x, const int mouse_y)
{
    mLevel->get_menu()->set_building_window(create_window());
    Building::on_click(mouse_x, mouse_y);
}
