#include "MainMenuTab.h"
#include "LayerHandler.h"
#include "Button.h"

MainMenuTab::MainMenuTab(Game* game) : Renderable(OVERLAY), mBackground_sprite(nullptr), mGame(game)
{
}

void MainMenuTab::render()
{
	SDL_Rect dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = 1920;
	dest.h = 1080;

	gLayer_handler->render_to_layer(mBackground_sprite, mRender_layer, nullptr, &dest);
}

void MainMenuTab::set_enabled(bool enabled)
{
	for(auto button : mButtons)
	{
		button->set_rendering_enabled(enabled);
		if (enabled) button->enable();
		else button->disable();
	}
	this->set_rendering_enabled(enabled);
}



