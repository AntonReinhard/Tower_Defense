#include "BuildingMenuItem.h"
#include "ConfigFile.h"
#include "MouseHandler.h"
#include "BuildingMouseItem.h"
#include "Menu.h"
#include <strstream>

BuildingMenuItem::BuildingMenuItem(const std::string& name_of_object, Level* level, const SDL_Point coords, LAYERS click_layer, LAYERS render_layer)
	: MenuItem(name_of_object, level, coords, click_layer, render_layer)
{
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
	mConstruction_costs.set_resources(gConfig_file->value_or_zero(building_stats_section, "goldcosts"),
		gConfig_file->value_or_zero(building_stats_section, "woodcosts"),
		gConfig_file->value_or_zero(building_stats_section, "stonecosts"),
		gConfig_file->value_or_zero(building_stats_section, "ironcosts"),
		gConfig_file->value_or_zero(building_stats_section, "energycosts"),
		gConfig_file->value_or_zero(building_stats_section, "watercosts"),
		gConfig_file->value_or_zero(building_stats_section, "foodcosts"));

	Resources storage_limit{
		gConfig_file->value_or_zero(building_stats_section, "goldLimit"),
		gConfig_file->value_or_zero(building_stats_section, "woodLimit"),
		gConfig_file->value_or_zero(building_stats_section, "stoneLimit"),
		gConfig_file->value_or_zero(building_stats_section, "ironLimit"),
		gConfig_file->value_or_zero(building_stats_section, "energyLimit"),
		gConfig_file->value_or_zero(building_stats_section, "waterLimit"),
		gConfig_file->value_or_zero(building_stats_section, "foodLimit")
	};

	Resources maintenance{
		gConfig_file->value_or_zero(building_stats_section, "goldMain"),
		gConfig_file->value_or_zero(building_stats_section, "woodMain"),
		gConfig_file->value_or_zero(building_stats_section, "stoneMain"),
		gConfig_file->value_or_zero(building_stats_section, "ironMain"),
		gConfig_file->value_or_zero(building_stats_section, "energyMain"),
		gConfig_file->value_or_zero(building_stats_section, "waterMain"),
		gConfig_file->value_or_zero(building_stats_section, "foodMain")
	};

	const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
	SDL_Color text_color = { 0, 0, 0 ,0 };
	SDL_Rect rect{ 1280, 364, 400, 220 };
	menu_item_window = std::make_shared<Window>(rect, WINDOWS, WINDOWS);
	rect.x += 20;
	rect.y += 20;
	rect.w = 0;//no scaling on text
	rect.h = 0;

	if (kind_of_object == "path")
	{
		auto speed_multiplier = gConfig_file->value(building_stats_section, "speed_multiplier");
		menu_item_window->add_text_to_window(new Text("Speed Multi", rect, WINDOWCONTENT, text_color, false));
		rect.x += 120;
		menu_item_window->add_text_to_window(new Text(std::to_string(speed_multiplier), rect, WINDOWCONTENT, text_color, false));
	}
	else
	{
		menu_item_window->add_text_to_window(new Text(mName_of_object, rect, WINDOWCONTENT, text_color, false));
		rect.y += 20;
		menu_item_window->add_text_to_window(new Text("       MaxStorage   Mainten", rect, WINDOWCONTENT, text_color, false));

		rect.x += 60;
		for (auto i = 0; i < RESOURCES_TOTAL; ++i)
		{
			rect.y += 20;
			menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(storage_limit.get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color, false));
			rect.x += 70;
			menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(maintenance.get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color, false));
			rect.x -= 70;
			menu_item_window->add_text_to_window(new Text(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color, false));
		}
		rect = { menu_item_window->get_dim().x + 220, menu_item_window->get_dim().y + 40, 0, 0 };

		if (kind_of_object == "onhittower" || kind_of_object == "aoetower" || kind_of_object == "statictargettower")
		{
			const Damage damage{ gConfig_file->value_or_zero(building_stats_section, "phys"),
				gConfig_file->value_or_zero(building_stats_section, "magic"),
				gConfig_file->value_or_zero(building_stats_section, "fire"),
				gConfig_file->value_or_zero(building_stats_section, "water"),
				gConfig_file->value_or_zero(building_stats_section, "elec") };
			const auto range = gConfig_file->value(building_stats_section, "range");
			const auto attack_speed = gConfig_file->value(building_stats_section, "attackspeed");

			menu_item_window->add_text_to_window(new Text("Dmg: ", rect, WINDOWS, text_color, false));
			rect.y += 30;
			menu_item_window->add_text_to_window(new Text("AS: ", rect, WINDOWS, text_color, false));
			rect.y += 30;
			menu_item_window->add_text_to_window(new Text("Range: ", rect, WINDOWS, text_color, false));
			rect.y += 30;
			menu_item_window->add_text_to_window(new Text("Damage dist: ", rect, WINDOWS, text_color, false));
			rect.y += 30;
			//damage distribution text
			menu_item_window->add_text_to_window(
				new Text("P: " + Text::remove_trailing_zeros(std::to_string(damage.get_phys_dmg()))
					+ " M: " + Text::remove_trailing_zeros(std::to_string(int(damage.get_magic_dmg())))
					+ " F: " + Text::remove_trailing_zeros(std::to_string(damage.get_fire_dmg()))
					+ " W: " + Text::remove_trailing_zeros(std::to_string(damage.get_water_dmg()))
					+ " E: " + Text::remove_trailing_zeros(std::to_string(damage.get_elec_dmg())), rect, WINDOWS, text_color, false));
			rect.x = menu_item_window->get_dim().x + 280;
			rect.y = menu_item_window->get_dim().y + 40;

			menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(damage.get_dmg_sum())), rect, WINDOWS, text_color, false));
			rect.y += 30;
			menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(attack_speed)), rect, WINDOWS, text_color, false));
			rect.y += 30;
			menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(range)), rect, WINDOWS, text_color, false));

			if (kind_of_object == "aoetower")
			{
				const auto explosive_radius = gConfig_file->value(building_stats_section, "explosiveradius");
				rect.x = menu_item_window->get_dim().x + 220;
				rect.y = menu_item_window->get_dim().y + 160;

				menu_item_window->add_text_to_window(new Text("Explosive Radius: ", rect, WINDOWCONTENT, text_color, false));
				rect.x += 130;
				menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(explosive_radius)), rect, WINDOWCONTENT, text_color, false));
			}
		}
		else if (kind_of_object == "industrialbuilding")
		{
			Resources produce{
				gConfig_file->value_or_zero(building_stats_section, "goldproduction"),
				gConfig_file->value_or_zero(building_stats_section, "woodproduction"),
				gConfig_file->value_or_zero(building_stats_section, "stoneproduction"),
				gConfig_file->value_or_zero(building_stats_section, "ironproduction"),
				gConfig_file->value_or_zero(building_stats_section, "energyproduction"),
				gConfig_file->value_or_zero(building_stats_section, "waterproduction"),
				gConfig_file->value_or_zero(building_stats_section, "foodproduction")
			};

			menu_item_window->add_text_to_window(new Text("Production", rect, WINDOWCONTENT, text_color, false));

			for (auto i = 0; i < RESOURCES_TOTAL; ++i)
			{
				rect.y += 20;
				menu_item_window->add_text_to_window(new Text(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color, false));
				rect.x += 60;
				menu_item_window->add_text_to_window(new Text(Text::remove_trailing_zeros(std::to_string(produce.get_display_resources().get_resource(RESOURCETYPES(i)))), rect, WINDOWCONTENT, text_color, false));
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
