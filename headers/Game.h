#pragma once
#include <vector>
#include "Tower.h"
#include "IndustrialBuilding.h"
#include "Menu.h"
#include "Map.h"

class Game 
{
public:
	Game();
	~Game();

	void start_game();
	void render_all() const;

	void add_tower(Tower* tower);
	void add_industrial_building(IndustrialBuilding* industrial_building);

private:
	std::vector<Tower*> mAll_towers;
	std::vector<IndustrialBuilding*> mAll_industrial_buildings;
	std::vector<Level*> mLevels;
	Level* mCurrent_level;
	Map* mMap;
	SDL_Point mMouse_position;
};
