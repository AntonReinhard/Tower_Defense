#include "AoeTower.h"
#include "AoeShot.h"
#include "ConfigFile.h"

AoeTower::AoeTower(const std::string& tower_name, const SDL_Point coords, Level* level, LAYERS const click_layer, LAYERS const render_layer) : Tower(tower_name, coords, level, click_layer, render_layer)
{
	mExplosive_radius = gConfig_file->value(mTower_name + "/stats", "explosiveradius");

	//add explosive radius text into window 
	SDL_Color text_color = { 0,0,0,0 };
	SDL_Rect dest;
	dest.x = mBuilding_window->get_dim().x + 200;
	dest.y = mBuilding_window->get_dim().y + 160;
	dest.h = 0;
	dest.w = 0;
	auto const explosive_radius_name = new Text("Explosive radius: ", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(explosive_radius_name);
	mExplosive_radius_value = new Text(std::to_string(int(mExplosive_radius)), dest, WINDOWCONTENT, text_color, mBuilding_window);
	mExplosive_radius_value->add_x_dim(130);
	mBuilding_window->add_text_to_window(mExplosive_radius_value);
}

void AoeTower::create_shot(Enemy* enemy)
{
	new AoeShot(this, enemy->get_position(), mExplosive_radius);
}

void AoeTower::on_tick()
{
	update_building_window();
	Tower::on_tick();
}

void AoeTower::update_building_window()
{
	Tower::update_building_window();
	mExplosive_radius_value->set_text(std::to_string(int(mExplosive_radius)));
}
