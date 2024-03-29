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
        mProduction_values[i]->set_text(std::to_string(mProduce.get_display(RESOURCETYPES(i))));
    }
}

std::shared_ptr<Window> IndustrialBuilding::create_window()
{
    Building::create_window();

    //add production text in the building window
    const SDL_Color text_color = { 0,0,0,0 };
    SDL_Rect dest{ 260, 40, 0, 0 };
    mBuilding_window->add_text_to_window("        Production", dest, WINDOWCONTENT, text_color);

    for (auto i = 0; i < mProduction_values.size(); ++i)
    {
        dest.y += 20;
        mProduction_values[i] = mBuilding_window->add_text_to_window(std::to_string(mProduce.get_display(RESOURCETYPES(i))), dest, WINDOWCONTENT, text_color);
    }

    return mBuilding_window;
}

void IndustrialBuilding::on_tick()
{
    mIdle = !mCurrent_resources.can_sub(mMaintenance);
    Building::on_tick();
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
