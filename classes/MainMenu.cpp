#include "MainMenu.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "Game.h"
#include "LayerHandler.h"

MainMenu::MainMenu(Game* game) : MainMenuTab(game)
{
	const auto sprite_section = "MainMenu/sprite";

	mBackground_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "background"));
	mTitle_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "title"));
	mMain_menu_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "mainmenu"));

	SDL_Rect dim {
		LOGICAL_SCREEN_WIDTH / 2 - gConfig_file->value("button/play", "clip_width"),
		300,
		gConfig_file->value("button/play", "clip_width"),
		gConfig_file->value("button/play", "clip_height")
	};

	mButtons.push_back(new Button("play", dim, this, OVERLAY, OVERLAY, PLAY));

	dim.w = gConfig_file->value("button/quit", "clip_width");
	dim.h = gConfig_file->value("button/quit", "clip_height");
	dim.y = 500;
	mButtons.push_back(new Button("quit", dim, this, OVERLAY, OVERLAY, QUIT));

	dim.w = gConfig_file->value("button/settings", "clip_width");
	dim.h = gConfig_file->value("button/settings", "clip_height");
	dim.y = 700;
	auto settings_button = new Button("settings", dim, this, OVERLAY, OVERLAY, SETTINGS);
	mButtons.push_back(settings_button);

	dim.w = gConfig_file->value("button/stats", "clip_width");
	dim.h = gConfig_file->value("button/stats", "clip_height");
	dim.y = 900;
	auto stats_button = new Button("stats", dim, this, OVERLAY, OVERLAY, STATS);
	mButtons.push_back(stats_button);

	this->set_enabled(true);
}

void MainMenu::on_button_press(int button_id, Button* button)
{
	switch (button_id)
	{
		case int(PLAY) :
			this->set_enabled(false);

			mGame->set_state(Game::STATE::LEVEL_SELECT);
			mGame->get_level_select_menu()->set_enabled(true);

			break;

			case int(QUIT) :
				mGame->set_state(Game::STATE::EXITING);
				break;

				case int(SETTINGS) :
					break;

					case int(STATS) :
						break;

						case int(BACK) :
							this->set_enabled(true);

							break;

						default:
							printf("This button doesn't exist?!\n\r");
							break;
	}
}


