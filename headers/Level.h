#pragma once
#include <vector>

#include "Wave.h"
#include "Resources.h"
#include "Enums.h"
#include "Map.h"

class Menu;
class Building;
class Warehouse;
class Game;

class Level
{
public:
	Level(std::string level_number, Game* game);
	~Level();

	//at the moment: update the first wave in the vector, if this wave is dead spawn the next wave
	void on_tick();

	bool is_dead() const;
	bool no_lives() const;

	std::vector<Wave*> get_waves();
	int get_lives() const;
	Resources* get_resources() const;
	std::string get_level_number() const;

	void set_resources(const Resources *resources);
	void set_lives(int lives);
	TILETYPES get_map_matrix(int x, int y) const;
	void set_map_matrix(int x, int y, TILETYPES type) const;

	void set_building_matrix(int x, int y, Building* building, int x_size = 1, int y_size = 1) const;
	Building* get_building_matrix(int x, int y) const;

	Warehouse* get_main_building() const;
	void set_main_building(Warehouse *main_building);

	Menu* get_menu() const;
	Map* get_map() const;
	
private:
	std::string mLevel_number;

	int mWave_number; //next wave to create
	
	std::vector<Wave*> mWaves;
	int mLives;
	Resources mStart_resources;
	Building*** mMap_buildings;

	Warehouse* mMain_building;

	Map* mMap;

	bool mDeleting;

	Game* mGame;

	Menu* mMenu;
};
