#pragma once
#include <vector>
#include "ButtonObject.h"
#include "LTexture.h"
#include "Renderable.h"

class Game;

class MainMenuTab : public Renderable
{
public:
	MainMenuTab(Game* game);

	void render() override;
	
	void set_enabled(bool enabled);

protected:	
	LTexture* mBackground_sprite;

	std::vector<Button*> mButtons;

	Game* mGame;
};
