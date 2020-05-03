#pragma once
#include "ButtonObject.h"
#include "MainMenuTab.h"

class LevelSelectMenu : public MainMenuTab, ButtonObject
{
public:
	LevelSelectMenu(Game* game);

	void on_button_press(int button_id, Button* button) override;

	//extra buttons should be added before the "LEVEL" entry
	enum LEVEL_SELECT_BUTTONS
	{
		BACK,
		LEVEL,
		TOTAL
	};

private:
};
