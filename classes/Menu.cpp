#include "Menu.h"
#include <SDL.h>
#include "LTexture.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include <functional>
#include "SDL_setup.h"

void Menu::show_towers()
{
	this->mOpen_tab = TOWER_TAB;
}

void Menu::show_industrial_buildings()
{
	this->mOpen_tab = INDUSTRIAL_BUILDING_TAB;
}

void Menu::on_button_press(int button_id)
{
	if (button_id == BUTTONIDS::TOWERS_BUTTON) {
		this->show_towers();
	}
	else if (button_id == BUTTONIDS::INDUSTRIAL_BUILDINGS_BUTTON) {
		this->show_industrial_buildings();
	}
}

Menu::Menu(Level *level)
{
	this->mLevel = level;
	this->mMenu_texture = nullptr;
	this->mOpen_tab = TOWER_TAB;
	SDL_Rect dim;
	dim.x = 1300;
	dim.y = 0;
	dim.w = 100;
	dim.h = 40;
	mTab_tower_button = new Button("TowerButton", dim, this, BUTTONIDS::TOWERS_BUTTON);
	dim.x = 1400;
	mTab_industrial_buildings_button = new Button("TowerButton", dim, this, BUTTONIDS::INDUSTRIAL_BUILDINGS_BUTTON);

	this->sort_items_into_menu();
}

Menu::~Menu()
{
}

void Menu::sort_items_into_menu()
{
	SDL_Point coords;
	std::string name_of_object;
	auto const number_of_items_in_one_row = 5;

	for (auto i = 0; ; i++)
	{
		coords.x = 1300 + (i % number_of_items_in_one_row) * TILE_WIDTH;
		coords.y = 64 + (i / number_of_items_in_one_row) * TILE_HEIGHT;

		name_of_object.assign(gConfig_file->Value("tower", std::to_string(i)));
		if (name_of_object == "end")
		{
			break;
		}
		auto new_item = new MenuItem(name_of_object, mLevel, coords);
		this->add_menu_item_tower(new_item);
	}

	for (auto i = 0; ; i++)
	{
		coords.x = 1300 + (i % number_of_items_in_one_row) * TILE_WIDTH;
		coords.y = 64 + (i / number_of_items_in_one_row) * TILE_HEIGHT;

		name_of_object.assign(gConfig_file->Value("industrialbuildings", std::to_string(i)));
		if (name_of_object == "end")
		{
			break;
		}
		auto new_item = new MenuItem(name_of_object, mLevel, coords);
		this->add_menu_item_industrialbuilding(new_item);
	}
}


void Menu::render()
{
	SDL_Color text_color = { 0, 0, 255 };

	//if there is a texture from last time, delete it
	if (mMenu_texture != nullptr)
	{
		mMenu_texture->free();
		delete mMenu_texture;
	}

	this->mMenu_texture = new LTexture();
	
	mMenu_texture->load_from_rendered_text("lives: " + std::to_string(mLevel->get_lives()) + " \tgold: " + std::to_string(mLevel->get_resources()->get_gold()) +
		" \twood: " + std::to_string(mLevel->get_resources()->get_wood()) + " \tstone: " + std::to_string(mLevel->get_resources()->get_stone()) +
		" \tiron: " + std::to_string(mLevel->get_resources()->get_iron()) + " \tenergy: " + std::to_string(mLevel->get_resources()->get_energy()) +
		" \twater: " + std::to_string(mLevel->get_resources()->get_water()) + " \tfood: " + std::to_string(mLevel->get_resources()->get_food()), text_color);
	
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 1050;
	dest.w = this->mMenu_texture->get_width();
	dest.h = this->mMenu_texture->get_height();

	gLayer_handler->render_to_layer(this->mMenu_texture, LAYERS::WINDOWS, nullptr, &dest);

	if(this->mOpen_tab == TOWER_TAB)
	{
		for (auto i = 0; i < mMenu_items_tower.size(); i++)
		{
			mMenu_items_tower.at(i)->set_rendering_enabled(true);
		}
	}
	else
	{
		for (auto i = 0; i < mMenu_items_tower.size(); i++)
		{
			mMenu_items_tower.at(i)->delete_clickable_space();
			mMenu_items_tower.at(i)->set_rendering_enabled(false);
		}
	}
	if (this->mOpen_tab == INDUSTRIAL_BUILDING_TAB)
	{
		for (auto i = 0; i < mMenu_items_industrial_buildings.size(); i++)
		{
			mMenu_items_industrial_buildings.at(i)->set_rendering_enabled(true);
		}
	}
	else
	{
		for (auto i = 0; i < mMenu_items_industrial_buildings.size(); i++)
		{
			mMenu_items_industrial_buildings.at(i)->delete_clickable_space();
			mMenu_items_industrial_buildings.at(i)->set_rendering_enabled(false);
		}
	}

}

void Menu::add_menu_item_tower(MenuItem* menu_item)
{
	mMenu_items_tower.push_back(menu_item);
}


void Menu::add_menu_item_industrialbuilding(MenuItem* menu_item)
{
	mMenu_items_industrial_buildings.push_back(menu_item);
}

