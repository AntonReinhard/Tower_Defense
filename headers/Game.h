#pragma once
#include <vector>
#include "Tower.h"
#include "IndustrialBuilding.h"
#include "LevelSelectMenu.h"
#include "MainMenu.h"

class Game 
{
public:
	Game();
	~Game();

	void start_game();
	void render_all() const;

	void load_level(int level_number);

	enum class STATE
	{
		MAIN_MENU,
		PLAYING,
		LEVEL_SELECT,
		EXITING
	};

	void set_state(Game::STATE state);

	MainMenu* get_main_menu();
	LevelSelectMenu* get_level_select_menu();

private:

	STATE mState;
		
	Level* mCurrent_level;
	MainMenu* mMain_menu;
	LevelSelectMenu* mLevel_select_menu;

	SDL_Point mMouse_position;

	Text mAverage_fps;
	bool mVsync_enabled;
};
