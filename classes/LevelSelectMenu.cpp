#include <filesystem>
#include <fstream>
#include <sstream>
#include <google/protobuf/util/json_util.h>

#include "LevelSelectMenu.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Button.h"
#include "Game.h"

LevelSelectMenu::LevelSelectMenu(Game* game) : MainMenuTab(game)
{
	const auto sprite_section = "LevelSelectMenu/sprite";

	mBackground_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "background"));

	SDL_Rect dim;
	dim.w = gConfig_file->value("button/play", "clip_width");
	dim.h = gConfig_file->value("button/play", "clip_height");
	//start location and distance between each button, which can be clicked to start the individual levels
	const auto first_button_x_location = 100;
	const auto first_button_y_location = 100;
	const auto distance_between_buttons_x = 400;
	const auto distance_between_buttons_y = 200;
	dim.x = first_button_x_location;
	dim.y = first_button_y_location;

	auto i = 0;
	for (const auto& p : std::filesystem::directory_iterator("level"))
	{
		std::ifstream file(p.path());
		if (!file.is_open())
		{
			std::cerr << "Couldn't open map at " << p.path() << std::endl;
		}
		std::stringstream json_string;

		json_string << file.rdbuf();

		towerdefense::map check_valid;
		auto status = google::protobuf::util::JsonStringToMessage(json_string.str(), &check_valid);

		if (status.ok())
		{
			auto new_level_button = new Button("play", dim, this, OVERLAY, OVERLAY, LEVEL+i);
			mButtons.push_back(new_level_button);
			dim.x += distance_between_buttons_x;
			if (dim.x+dim.w > LOGICAL_SCREEN_WIDTH)
			{
				dim.x = first_button_x_location;
				dim.y += distance_between_buttons_y;
			}
		}
		else {
			std::cerr << "Couldn't load map at " << p.path() << std::endl;
		}
		++i;
	}
	dim.x = LOGICAL_SCREEN_WIDTH - dim.w - 100;
	dim.y = LOGICAL_SCREEN_HEIGHT - dim.h - 100;
	auto back_button = new Button("quit", dim, this, OVERLAY, OVERLAY, BACK);
	mButtons.push_back(back_button);
	this->set_enabled(false);
}

void LevelSelectMenu::on_button_press(int button_id, Button* button)
{
	this->set_enabled(false);
	//button ids in range from 0 to LEVEL_SELECT_BUTTONS::LEVEL - 1 are for buttons that are not "level buttons"
	if(button_id == BACK)
	{
		this->set_enabled(false);
		mGame->set_state(Game::STATE::MAIN_MENU);
		mGame->get_main_menu()->set_enabled(true);
	}
	//button ids in range from LEVEL_SELECT_BUTTONS::LEVEL to (LEVEL_SELECT_BUTTONS::LEVEL + number of levels - 1) are for all level buttons
	//that way new levels that are added, do not need an own entry in the LEVEL_SELECT_BUTTONS enum and the level number can be derived back from the button_id (button_id - LEVEL + 1)
	else if(button_id >= LEVEL)
	{
		this->set_enabled(false);
		mGame->load_level(button_id - LEVEL + 1);
		mGame->set_state(Game::STATE::PLAYING);
	}
}
