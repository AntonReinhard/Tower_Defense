#pragma once
#include "Clickable.h"
#include "LTexture.h"
#include "Entity.h"
#include "Menu.h"
#include "Enums.h"
#include "BigUpgrade.h"
#include "Production.h"
#include "Resources.h"

#include <SDL.h>

#include <string>
#include <array>
#include <optional>
#include <memory>

class Carriage;
class Level;
class Window;

class Building : public Clickable, public Entity, public ButtonObject
{
public:
    Building(std::string building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer);
    ~Building();

    void render() override; //render the picture of the building
    void on_tick() override; //takes resources for maintenance
    void demolish() const;
    virtual bool upgrade(const std::string& building_upgrade_section); //upgrades building

    SDL_Point get_coords() const;
    SDL_Rect get_dimensions() const;

    const Resources& get_maintenance() const;
    void set_maintenance(const Resources& maintenance);

    void set_coords(SDL_Point coords);
    void set_idle(bool value);
    bool get_idle() const;
    void set_building_level(std::string building_level);
    std::string get_building_level() const;
    int get_count_of_little_upgrades() const;
    std::string get_name() const;

    ENTITYTYPE get_type() override;
    virtual BUILDINGTYPE get_building_type() = 0;

    //returns the buildings neighboring this one
    std::vector<Building*> get_neighbors() const;
    void update_neighbors();

    const Resources& get_current_resources() const&;
    Resources& get_current_resources() &;
    void add_resources(const Resources& r);

    const Resources& get_produce() const;
    void set_produce(const Resources& produce);

    void transfer_resources(Resources& r, Production& production, bool reverse);

    //give window for building to the menu, so it can be shown there
    void on_click(int mouse_x, int mouse_y) override;

    virtual std::shared_ptr<Window> create_window();
    
    virtual void update_building_window();
    void update_great_upgrades();
    virtual void set_stat_strings_for_upgrade_buttons(UpgradeButton& button);
    void on_button_press(int button_id, Button& button) override;
    void upgrade_building(UpgradeButton& button);//button must be from class UpgradeButton
    void show_more(Button& button); //

    bool is_destroyable() const;
    void set_destroyable(bool destroyable);
    
protected:
    SDL_Point mCoords;
    SDL_Point mBuilding_dimensions{};
    Resources mMaintenance;
    Resources mProduce;
    LTexture *mSprite; //texture
    SDL_Rect mSprite_dimensions; //size of the texture
    Level *mLevel;

    bool mIdle; // if the player doesn't have enough res to maintain the building the building doesn't do anything
    int mElapsed_ticks;
    std::string mName;
    std::string mSprite_path;
    Resources mConstruction_costs;
    TILETYPES mTile_to_build_on;

    //a vector holding the neighbors of this building
    std::vector<Building*> mSurrounding_buildings;

    //how many resources the building has
    Resources mCurrent_resources;
    Production mProducing;

    std::string mBuilding_level; //level of the building, can be changed by GUpgrades
    int mCount_of_little_upgrades; //count of the little upgrades as a requirement for big upgrades

    Carriage* mCarriage;

    std::shared_ptr<Window> mBuilding_window;
    std::array<std::shared_ptr<Text>, RESOURCES_TOTAL> mStorage_values;
    std::array<std::shared_ptr<Text>, RESOURCES_TOTAL> mMaintenance_values;
    std::vector<BigUpgrade*> mBig_upgrades;
    Vector mButton_offset;

    bool mIs_destroyable;
};
