#pragma once
#include "LTexture.h"
#include "Button.h"
#include "MainMenuTab.h"

class Game;

class MainMenu : public MainMenuTab, public ButtonObject
{
public:
	explicit MainMenu(Game* game);

	void on_button_press(int button_id, Button* button) override;

	enum MAIN_MENU_BUTTONS
	{
		PLAY,
		QUIT,
		SETTINGS,
		STATS,
		BACK
	};

private:
	LTexture *mTitle_sprite;
	LTexture *mMain_menu_sprite;
};
