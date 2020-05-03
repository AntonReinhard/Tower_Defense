#include <utility>

#include "Level.h"
#include "Building.h"
#include "ConfigFile.h"
#include "WareHouse.h"
#include "SDL_setup.h"
#include "Map.h"
#include "EntityHandler.h"
#include "Game.h"
#include "MouseHandler.h"

Level::Level(std::string level_number, Game* game) : mLevel_number(std::move(level_number)), mMain_building(), mDeleting(false), mGame(game)
{
	//mLevel_number = level_number;
	const auto level_section = "level" + mLevel_number;
	mLives = gConfig_file->value_or_zero(level_section, "lives");

	//set the start-resources in this level
	mStart_resources.set_resources(gConfig_file->value_or_zero("level" + mLevel_number, "gold"),
		gConfig_file->value_or_zero(level_section, "wood"),
		gConfig_file->value_or_zero(level_section, "stone"),
		gConfig_file->value_or_zero(level_section, "iron"),
		gConfig_file->value_or_zero(level_section, "energy"),
		gConfig_file->value_or_zero(level_section, "water"),
		gConfig_file->value_or_zero(level_section, "food"));

	mWave_number = 1;
	const auto first_wave = new Wave(std::to_string(mWave_number), this);
	mWaves.push_back(first_wave);
	mWave_number++;

	std::string content;

	this->mMap_buildings = new Building * *[MATRIX_WIDTH];
	for (auto x = 0; x < MATRIX_WIDTH; x++)
	{
		this->mMap_buildings[x] = new Building * [MATRIX_HEIGHT];
		for (auto y = 0; y < MATRIX_HEIGHT; y++)
		{
			this->mMap_buildings[x][y] = nullptr;
		}
	}
	
	//has to happen after mMap_buildings was initialized
	SDL_Point warehouse_coord;
	warehouse_coord.x = TILE_WIDTH * gConfig_file->value(level_section, "main_building_x");
	warehouse_coord.y = TILE_HEIGHT * gConfig_file->value(level_section, "main_building_y");
	mMain_building = new Warehouse(gConfig_file->value(level_section, "main_building_name"), warehouse_coord, this, BUILDINGS, BUILDINGS);

	mMenu = new Menu(this, LAYERS::BACKGROUND);

	mMap = new Map("level/" + std::string(gConfig_file->value(level_section, "map_file")));

	const auto r = new Resources(
		gConfig_file->value(level_section, "gold"),
		gConfig_file->value(level_section, "wood"),
		gConfig_file->value(level_section, "stone"),
		gConfig_file->value(level_section, "iron"),
		gConfig_file->value(level_section, "energy"),
		gConfig_file->value(level_section, "water"),
		gConfig_file->value(level_section, "food")
	);
	
	mMain_building->add_resources(r);
	//can't destroy the main building
	mMain_building->set_destroyable(false);
}

Level::~Level()
{
	mDeleting = true;

	gEntity_handler->delete_all_entities();

	delete mMap;

	delete mMenu;

	mGame->set_state(Game::STATE::MAIN_MENU);

	gMouse_handler->set_item_on_mouse(nullptr);
}

void Level::on_tick()
{
	auto last_wave_did_start = true; //is set true, if the wave before is spawning their monsters, otherwise it is false; for the first wave always true

	//collecting waves to be deleted
	std::vector<int> deleted_waves;;

	//need to go through backwards so we can delete waves from the vector
	for (std::size_t i = 0; i < mWaves.size(); ++i)
	{
		auto wave = mWaves[i];

		if (last_wave_did_start) wave->update();

		last_wave_did_start = wave->get_elapsed_ticks() >= wave->get_spawn_delay();

		if (last_wave_did_start && gConfig_file->value_exists("wave" + mLevel_number + "_" + std::to_string(mWave_number), "exists")
			&& wave->get_wave_number() == std::to_string(mWave_number - 1))
		{
			auto new_wave = new Wave(std::to_string(mWave_number), this);
			mWaves.push_back(new_wave);
			mWave_number++;
		}

		if (wave->is_dead())
		{
			delete wave;
			deleted_waves.push_back(i);
		}
	}

	//deleting the waves that died
	auto count = 0;
	for (auto wave : deleted_waves)
	{
		mWaves.erase(mWaves.begin() + wave - count);
		++count;
	}
}

bool Level::is_dead() const
{
	return mWaves.empty();
}

bool Level::no_lives() const
{
	return mLives <= 0;
}

int Level::get_lives() const
{
	return mLives;
}

std::vector<Wave*> Level::get_waves()
{
	return mWaves;
}

Resources* Level::get_resources() const
{
	return this->mMain_building->get_current_resources();
}

void Level::set_resources(const Resources* resources)
{
	mStart_resources = *resources;
}

std::string Level::get_level_number() const
{
	return this->mLevel_number;
}

void Level::set_lives(const int lives)
{
	mLives = lives;
}

TILETYPES Level::get_map_matrix(const int x, const int y) const
{
	return mMap->get_resource_at_tile(x, y);
}

void Level::set_building_matrix(const int x, const int y, Building* building, const int x_size, const int y_size) const
{
	if (x < 0 || x + x_size > MATRIX_WIDTH || y < 0 || y + y_size > MATRIX_HEIGHT) return;

	//set all the fields occupied
	for (auto x_i = 0; x_i < x_size; x_i++)
		for (auto y_i = 0; y_i < y_size; y_i++)
			this->mMap_buildings[x + x_i][y + y_i] = building;

	//update building's neighbors
	//up side
	if (y > 0)
	{
		for (auto x_i = 0; x_i < x_size; x_i++)
		{
			if (get_building_matrix(x + x_i, y - 1) != nullptr)
				get_building_matrix(x + x_i, y - 1)->update_neighbors();
		}
	}

	//bottom side
	if (y < MATRIX_HEIGHT - y_size)
	{
		for (auto x_i = 0; x_i < x_size; x_i++)
		{
			if (get_building_matrix(x + x_i, y + y_size) != nullptr)
				get_building_matrix(x + x_i, y + y_size)->update_neighbors();
		}
	}

	//left side
	if (x > 0)
	{
		for (auto y_i = 0; y_i < y_size; y_i++)
		{
			if (get_building_matrix(x - 1, y + y_i) != nullptr)
				get_building_matrix(x - 1, y + y_i)->update_neighbors();
		}
	}

	//right side
	if (x < MATRIX_WIDTH - x_size)
	{
		for (auto y_i = 0; y_i < y_size; y_i++)
		{
			if (get_building_matrix(x + x_size, y + y_i) != nullptr)
				get_building_matrix(x + x_size, y + y_i)->update_neighbors();
		}
	}
}

Building* Level::get_building_matrix(const int x, const int y) const
{
	if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) return nullptr;
	return this->mMap_buildings[x][y];
}

Warehouse* Level::get_main_building() const
{
	return mDeleting ? nullptr : this->mMain_building;
}

void Level::set_main_building(Warehouse* main_building)
{
	this->mMain_building = main_building;
	this->mMain_building->get_current_resources()->set_empty();
	this->mMain_building->get_current_resources()->add(&this->mStart_resources);
}

Menu* Level::get_menu() const
{
	return mMenu;
}

Map* Level::get_map() const
{
	return mMap;
}
