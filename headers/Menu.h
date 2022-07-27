#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "Button.h"
#include "ButtonObject.h"
#include "Constants.h"
#include "Text.h"
#include "DemolishTool.h"

class BuildingMenuItem;
class MenuItem;

class Menu final
    : public Renderable, public ButtonObject
{
public:
    explicit Menu(Level *level, LAYERS render_layer);
    ~Menu();

    void sort_items_into_menu();
    void show_tab(BUILDINGTYPE open_tab);
    void on_button_press(int button_id, Button& button) override;
    void add_menu_item(BuildingMenuItem* building_menu_item, BUILDINGTYPE tab);

    void render() override; //shows lives and resources

    void set_building_window(const std::shared_ptr<Window>& building_window);
    std::shared_ptr<Window> get_building_window() const;

    void set_unit_window(const std::shared_ptr<Window>& unit_window);
    std::shared_ptr<Window> get_unit_window() const;

    void set_menu_item_window(const std::shared_ptr<Window>& menu_item_window);
    std::shared_ptr<Window> get_menu_item_window() const;

private:
    Level* mLevel;

    std::map<BUILDINGTYPE, std::vector<BuildingMenuItem*>*> mBuilding_menu_items;
    std::map<BUILDINGTYPE, Button*> mButtons;

    std::shared_ptr<Window> mMenu_item_window; //window from the menu-item that was last clicked
    std::shared_ptr<Window> mBuilding_window; //window from the building that was last clicked
    std::shared_ptr<Window> mUnit_window; //window from the unit that was last clicked

    DemolishTool* mDemolish_tool;

    BUILDINGTYPE mOpen_tab;
};
