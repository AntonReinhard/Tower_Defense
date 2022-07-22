#include <utility>

#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "Window.h"
#include "LayerHandler.h"
#include "MouseHandler.h"
#include "Production.h"
#include "Carriage.h"
#include "BigUpgrade.h"
#include "BuildingMenuItem.h"

Building::Building(std::string building_name, const SDL_Point coords, Level* level, const LAYERS click_layer, const LAYERS render_layer)
    : Clickable(click_layer)
    , Entity(render_layer)
    , mCoords{ coords }
    , mSprite_dimensions{}
    , mLevel{ level }
    , mName{ std::move(building_name) }
    , mCarriage{ nullptr }
{
    const auto building_sprite_section = mName + "/sprite";
    const auto building_stats_section = mName + "/stats";
    mSprite_path = std::string(gConfig_file->value(building_sprite_section, "path"));

    //load texture and the size of the image from the config file
    mSprite = gTextures->get_texture(mSprite_path);
    mSprite_dimensions.w = gConfig_file->value(building_sprite_section, "image_width");
    mSprite_dimensions.h = gConfig_file->value(building_sprite_section, "image_height");
    mSprite_dimensions.x = 0;
    mSprite_dimensions.y = 0;

    // read all the resources in
    mCurrent_resources.read_from_config(building_stats_section, nullptr, "%sLimit");
    mConstruction_costs.read_from_config(building_stats_section, "%scosts");
    mMaintenance.read_from_config(building_stats_section, "%sMain");
    mProduce.read_from_config(building_stats_section, "%sproduction");

    mBuilding_level = "";
    mCount_of_little_upgrades = 0;

    mElapsed_ticks = 0;

    SDL_Rect clickable;
    clickable.x = coords.x;
    clickable.y = coords.y;
    clickable.w = mSprite_dimensions.w;
    clickable.h = mSprite_dimensions.h;
    this->set_clickable_space(clickable);
    mIdle = true;

    //set the mouse over window up with initial values
    int tile = gConfig_file->value(building_stats_section, "tile");
    mTile_to_build_on = static_cast<TILETYPES>(tile);

    //set the size of the building in tiles
    this->mBuilding_dimensions.x = gConfig_file->value_or_zero(building_stats_section, "size_x");
    this->mBuilding_dimensions.y = gConfig_file->value_or_zero(building_stats_section, "size_y");
    if (this->mBuilding_dimensions.x <= 0) mBuilding_dimensions.x = 1;
    if (this->mBuilding_dimensions.y <= 0) mBuilding_dimensions.y = 1;

    //make entry in the building matrix for this building
    mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, this, mBuilding_dimensions.x, mBuilding_dimensions.y);

    this->update_neighbors();

    //initialize what this building is producing and consuming
    mProducing = Production(*this);

    mIs_destroyable = true;
}

Building::~Building()
{
    delete mCarriage;
    //don't destroy texture, handled by texture class

    for (auto big_upgrade : mBig_upgrades)
    {
        delete big_upgrade;
    }
}

void Building::update_building_window()
{
    if (!this->mBuilding_window.get()) return;
    for (auto i = 0; i < RESOURCES_TOTAL; ++i)
    {
        mStorage_values[i]->set_text(std::to_string(mCurrent_resources.get_display(RESOURCETYPES(i)))
            + "/" + std::to_string(std::lround(mCurrent_resources.get_limit(RESOURCETYPES(i)))));
        mMaintenance_values[i]->set_text(Text::remove_trailing_zeros(std::to_string(mMaintenance.get_resource(RESOURCETYPES(i)))));
    }
    //changes string if a upgrade button is hovered
    for (auto upgrade : mBig_upgrades)
    {
        if (upgrade->get_big_upgrade_button()->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
        {
            set_stat_strings_for_upgrade_buttons(*upgrade->get_big_upgrade_button());
        }
    }
}

void Building::set_stat_strings_for_upgrade_buttons(UpgradeButton& button)
{

}

void Building::update_great_upgrades()
{
    //first delete old buttons
    if (!mBig_upgrades.empty())
    {
        for (auto& big_upgrade : mBig_upgrades)
        {
            delete big_upgrade;
        }
        mBig_upgrades.clear();
    }

    //then create new buttons
    SDL_Rect button_dim;
    button_dim.x = static_cast<int>(mBuilding_window->get_dim().x + mButton_offset.x);
    button_dim.y = static_cast<int>(mBuilding_window->get_dim().y + mButton_offset.y);
    button_dim.w = 26;
    button_dim.h = 26;

    for (auto i = 1; ; i++)
    {
        const auto gap_between_two_upgrades = 30;
        const auto y_difference = -160; // how much the Show-More-Button is away from the Big-Upgrade-Button on the y-Axis
        button_dim.y += gap_between_two_upgrades;
        const auto upgrade_section = get_building_level() + std::to_string(i);
        //check if upgrade exists
        //if upgrade exists, check if it is available in this level 
        if (!gConfig_file->value_exists(get_name() + "/upgrade" + upgrade_section, "exists") || 
            std::find(mLevel->get_available_upgrades().begin(), mLevel->get_available_upgrades().end(),
            (std::string)gConfig_file->value(this->get_name() + "/upgrade" + upgrade_section, "name")) ==
            mLevel->get_available_upgrades().end())
        {
            break;
        }
        
        const auto big_upgrade_button = new UpgradeButton("testbutton", button_dim, this, mName, upgrade_section, WINDOWCONTENT, WINDOWCONTENT, mBuilding_window.get(), UPGRADE_BUTTON);
        auto show_more_button = new ShowMoreButton("testbutton", button_dim, this, WINDOWCONTENT, WINDOWCONTENT, mBuilding_window.get(), SHOW_MORE_BUTTON);
        show_more_button->add_x_dimension(y_difference);
        show_more_button->set_clickable_space(show_more_button->get_dimension());
        auto big_upgrade = new BigUpgrade(mName, upgrade_section, big_upgrade_button, show_more_button);
        mBig_upgrades.push_back(big_upgrade);
    }
}

void Building::upgrade_building(UpgradeButton& button)
{
    const auto building_upgrade_section = mName + "/upgrade" + button.get_upgrade_section();
    if (gConfig_file->value_or_zero(building_upgrade_section, "count_of_little_upgrades") > mCount_of_little_upgrades)
    {
        return;
    }
    upgrade(building_upgrade_section);
    set_building_level(button.get_upgrade_section() + ".");
    update_great_upgrades();
}

void Building::show_more(Button& button)
{
    const auto value_to_shift = 60; // y-direction-shift
    if (!mBig_upgrades.empty())
    {
        for (auto big_upgrade : mBig_upgrades)
        {
            if (big_upgrade->is_upgrade_description_shown()) big_upgrade->set_upgrade_description_shown(false);
            if (big_upgrade->get_show_more_button() == &button) big_upgrade->set_upgrade_description_shown(true);

            if (big_upgrade->is_shifted_down())	big_upgrade->shift(-value_to_shift);
            if (big_upgrade->get_show_more_button()->get_dimension().y > button.get_dimension().y) big_upgrade->shift(value_to_shift);
        }
    }
}

bool Building::is_destroyable() const
{
    return mIs_destroyable;
}

void Building::set_destroyable(const bool destroyable)
{
    mIs_destroyable = destroyable;
}

void Building::on_button_press(const int button_id, Button& button)
{
    if (button_id == UPGRADE_BUTTON) upgrade_building(dynamic_cast<UpgradeButton&>(button));
    if (button_id == SHOW_MORE_BUTTON) show_more(button);
}

void Building::demolish() const
{
    auto refund = mConstruction_costs / 2.;
    mLevel->get_resources().add(refund);

    const auto tile_x = mCoords.x / TILE_WIDTH;
    const auto tile_y = mCoords.y / TILE_HEIGHT;

    mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, nullptr, mBuilding_dimensions.x, mBuilding_dimensions.y);
}

bool Building::upgrade(const std::string& building_upgrade_section)
{
    Resources upgrade_cost;
    upgrade_cost.read_from_config(building_upgrade_section, "%scosts");

    if (mLevel->get_resources().sub(upgrade_cost))
    {
        mConstruction_costs.add(upgrade_cost);

        Resources plus_maintenance;
        plus_maintenance.read_from_config(building_upgrade_section, "%sMain");

        mMaintenance.add(plus_maintenance);
        return true;
    }
    return false;
}

void Building::render()
{
    SDL_Rect dest;

    dest.x = mCoords.x;
    dest.y = mCoords.y;
    dest.w = mSprite_dimensions.w;
    dest.h = mSprite_dimensions.h;

    gLayer_handler->render_to_layer(mSprite, mRender_layer, {}, dest);
}

void Building::on_tick()
{
    if (mElapsed_ticks % *gFrame_rate == 0 && !mIdle)
    {
        mCurrent_resources.sub(mMaintenance);
        mCurrent_resources.add(mProduce);
    }
    mElapsed_ticks++;


    if (mBuilding_window != nullptr) update_building_window();
}

void Building::on_click(int mouse_x, int mouse_y)
{
    if (mBuilding_window == nullptr) create_window();
    mLevel->get_menu()->set_building_window(mBuilding_window);
}

std::shared_ptr<Window> Building::create_window()
{
    //building window
    mButton_offset.x = 560;
    mButton_offset.y = 20;
    const SDL_Color text_color = { 0, 0, 0 ,0 };
    SDL_Rect rect{ 1280, 584, 600, 220 };
    //window for the warehouse is at a different position, than the other buildingwindows
    if (get_name() != "Warehouse") //text for non-Warehouse buildings is at different position
    {
        //rect.x += rect.w - 200;
    }

    auto window = std::make_shared<Window>(rect, WINDOWS, WINDOWS);

    rect.x = 20;
    rect.y = 20;
    rect.w = 0;//no scaling on text
    rect.h = 0;

    window->add_text_to_window(mName, rect, WINDOWCONTENT, text_color);
    rect.y += 20;
    window->add_text_to_window(" Storage           Main", rect, WINDOWCONTENT, text_color);

    rect.x += 10;
    for (auto i = 0; i < RESOURCES_TOTAL; ++i)
    {
        rect.y += 20;
        mStorage_values[i] = window->add_text_to_window(std::to_string(mCurrent_resources.get_display(RESOURCETYPES(i)))
            + "/" + std::to_string(std::lround(mCurrent_resources.get_limit(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color);
        rect.x += 95;
        mMaintenance_values[i] = window->add_text_to_window(Text::remove_trailing_zeros(std::to_string(mMaintenance.get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color);
        rect.x += 30;
        window->add_text_to_window(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color);
        rect.x -= 125;
    }

    mBuilding_window = window;
    
    update_great_upgrades();
    
    return mBuilding_window;
}

void Building::set_maintenance(const Resources& maintenance)
{
    mMaintenance = maintenance;
}

void Building::set_coords(const SDL_Point coords)
{
    mCoords = coords;
}

SDL_Point Building::get_coords() const
{
    return mCoords;
}

SDL_Rect Building::get_dimensions() const
{
    return mSprite_dimensions;
}

const Resources& Building::get_maintenance() const
{
    return this->mMaintenance;
}

bool Building::get_idle() const
{
    return this->mIdle;
}

ENTITYTYPE Building::get_type()
{
    return ENTITYTYPE::BUILDING;
}

const Resources& Building::get_current_resources() const&
{
    return this->mCurrent_resources;
}

Resources& Building::get_current_resources()&
{
    return this->mCurrent_resources;
}

void Building::add_resources(const Resources& r)
{
    this->mCurrent_resources.add(r);
}

void Building::transfer_resources(Resources& r, Production& production, const bool reverse)
{
    if (!reverse) {
        mCurrent_resources.transfer(r, production);
    }
    else {
        r.transfer(mCurrent_resources, production);
    }
}

void Building::set_idle(const bool value)
{
    this->mIdle = value;
}

std::vector<Building*> Building::get_neighbors() const
{
    return this->mSurrounding_buildings;
}

void Building::update_neighbors()
{
    this->mSurrounding_buildings.clear();

    const auto x = mCoords.x / TILE_WIDTH;
    const auto y = mCoords.y / TILE_HEIGHT;

    const auto x_size = mBuilding_dimensions.x;
    const auto y_size = mBuilding_dimensions.y;

    //up side
    if (y > 0)
    {
        for (auto x_i = 0; x_i < x_size; x_i++)
        {
            auto building = mLevel->get_building_matrix(x + x_i, y - 1);
            if (building != nullptr)
                mSurrounding_buildings.push_back(building);
        }
    }

    //bottom side
    if (y < MATRIX_HEIGHT - y_size)
    {
        for (auto x_i = 0; x_i < x_size; x_i++)
        {
            auto building = mLevel->get_building_matrix(x + x_i, y + y_size);
            if (building != nullptr)
                mSurrounding_buildings.push_back(building);
        }
    }

    //left side
    if (x > 0)
    {
        for (auto y_i = 0; y_i < y_size; y_i++)
        {
            auto building = mLevel->get_building_matrix(x - 1, y + y_i);
            if (building != nullptr)
                mSurrounding_buildings.push_back(building);
        }
    }

    //right side
    if (x < MATRIX_WIDTH - x_size)
    {
        for (auto y_i = 0; y_i < y_size; y_i++)
        {
            auto building = mLevel->get_building_matrix(x + x_size, y + y_i);
            if (building != nullptr)
                mSurrounding_buildings.push_back(building);
        }
    }
}

std::string Building::get_building_level() const
{
    return mBuilding_level;
}

void Building::set_building_level(std::string building_level)
{
    mBuilding_level = building_level;
}

int Building::get_count_of_little_upgrades() const
{
    return mCount_of_little_upgrades;
}


std::string Building::get_name() const
{
    return mName;
}

void Building::set_produce(const Resources& produce)
{
    mProduce = produce;
}

const Resources& Building::get_produce() const
{
    return mProduce;
}
