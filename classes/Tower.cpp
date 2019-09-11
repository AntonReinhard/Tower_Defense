#include <SDL.h>
#include "Menu.h"
#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "EntityHandler.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "Carriage.h"


Tower::Tower(const std::string& tower_name, const SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : Building(tower_name, coords, level, click_layer, render_layer), mTower_name(tower_name)
{
	const auto tower_stats_section = mTower_name + "/stats";
	mDamage.set_damages(gConfig_file->value_or_zero(tower_stats_section, "phys"),
		gConfig_file->value_or_zero(tower_stats_section, "magic"),
		gConfig_file->value_or_zero(tower_stats_section, "fire"),
		gConfig_file->value_or_zero(tower_stats_section, "water"),
		gConfig_file->value_or_zero(tower_stats_section, "elec"));

	mRange = gConfig_file->value(tower_stats_section, "range");
	mAttack_speed = gConfig_file->value(tower_stats_section, "attackspeed");
	mProjectile_speed = gConfig_file->value(tower_stats_section, "projectilespeed");
	mProjectile_name.assign(gConfig_file->value(tower_stats_section, "projectile_name"));
	mAttack_cooldown = int(60 / mAttack_speed);
	const auto sprite_path = std::string(gConfig_file->value("rangeindicator", "path"));
	mRange_indicator_sprite = gTextures->get_texture(sprite_path);

	mCarriage = new Carriage("carriage", mLevel, LAYERS::ENEMIES, reinterpret_cast<Building*>(mLevel->get_main_building()), this);

	//little upgrade buttons
	SDL_Rect button_dim;
	button_dim.x = mBuilding_window->get_dim().x + 30;
	button_dim.y = mBuilding_window->get_dim().y + 140;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new UpgradeButton("testbutton", button_dim, this, mName, "Damage", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_range_button = new UpgradeButton("testbutton", button_dim, this, mName, "Range", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_attackspeed_button = new UpgradeButton("testbutton", button_dim, this, mName, "Attackspeed", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);

	//number of little upgrades displayed
	SDL_Color text_color = { 0,0,0,0 };
	SDL_Rect dest;
	dest.h = 0;
	dest.w = 0;
	dest.x = mUpgrade_damage_button->get_dimension().x;
	dest.y = mUpgrade_damage_button->get_dimension().y + 30;
	mDamage_upgrade_number_texture = new Text(std::to_string(mNumber_of_damage_upgrades), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mDamage_upgrade_number_texture);
	dest.x = mUpgrade_attackspeed_button->get_dimension().x;
	dest.y = mUpgrade_attackspeed_button->get_dimension().y + 30;
	mAttackspeed_upgrade_number_texture = new Text(std::to_string(mNumber_of_attackspeed_upgrades), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mAttackspeed_upgrade_number_texture);
	dest.x = mUpgrade_range_button->get_dimension().x;
	dest.y = mUpgrade_range_button->get_dimension().y + 30;
	mRange_upgrade_number_texture = new Text(std::to_string(mNumber_of_attackspeed_upgrades), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mRange_upgrade_number_texture);

	//turret stats-text displayed(const)
	dest.x = mBuilding_window->get_dim().x + 200;
	dest.y = mBuilding_window->get_dim().y + 20;
	dest.w = 0;	//setting these to 0 will not scale anything
	dest.h = 0;
	auto dmg_text = new Text("Dmg: ", dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(dmg_text);
	dest.y += 30;
	auto as_text = new Text("AS: ", dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(as_text);
	dest.y += 30;
	auto range_text = new Text("Range: ", dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(range_text);
	dest.y += 30;
	auto damage_distribution_headline = new Text("Damage dist: ", dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(damage_distribution_headline);
	dest.y += 30;
	mDamage_distribution_text = new Text("P: " + std::to_string(int(mDamage.get_phys_dmg()))
		+ " M: " + std::to_string(int(mDamage.get_magic_dmg()))
		+ " F: " + std::to_string(int(mDamage.get_fire_dmg()))
		+ " W: " + std::to_string(int(mDamage.get_water_dmg()))
		+ " E: " + std::to_string(int(mDamage.get_elec_dmg())), dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(mDamage_distribution_text);

	//turret stats-numbers displayed(dynamic)
	dest.x = mBuilding_window->get_dim().x + 260;
	dest.y = mBuilding_window->get_dim().y + 20;
	mDmg_value = new Text(std::to_string(int(mDamage.get_dmg_sum())), dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(mDmg_value);
	dest.y += 30;
	mAs_value = new Text(std::to_string(int(mAttack_speed)), dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(mAs_value);
	dest.y += 30;
	mRange_value = new Text(std::to_string(int(mRange)), dest, WINDOWS, text_color, this);
	mBuilding_window->add_text_to_window(mRange_value);
}

void Tower::render()
{
	Building::render();

	if (mLevel->get_menu()->get_building_window() != nullptr)
	{
		if (is_clicked())
		{
			SDL_Rect dest;
			//range is radius not diameter
			dest.x = int(get_coords().x - mRange + mSprite_dimensions.w / 2.);
			dest.y = int(get_coords().y - mRange + mSprite_dimensions.h / 2.);
			dest.w = int(mRange * 2);
			dest.h = int(mRange * 2);
			gLayer_handler->render_to_layer(mRange_indicator_sprite, LAYERS::OVERLAY, nullptr, &dest);
		}
	}
}

void Tower::update_building_window()
{
	Building::update_building_window();
	//updates texture: number of little upgrades
	mDamage_upgrade_number_texture->set_text(std::to_string(mNumber_of_damage_upgrades));
	mAttackspeed_upgrade_number_texture->set_text(std::to_string(mNumber_of_attackspeed_upgrades));
	mRange_upgrade_number_texture->set_text(std::to_string(mNumber_of_range_upgrades));

	//changes string if a upgradebutton is hovered
	auto is_a_button_hovered = false;
	for (auto& upgrade : mBig_upgrades)
	{
		if (upgrade->get_big_upgrade_button()->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		{
			set_stat_strings_for_upgrade_buttons(upgrade->get_big_upgrade_button());
			is_a_button_hovered = true;
		}
	}
	if (mUpgrade_damage_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_damage_button);
		is_a_button_hovered = true;
	}
	if (mUpgrade_attackspeed_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_attackspeed_button);
		is_a_button_hovered = true;
	}

	if (mUpgrade_range_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_range_button);
		is_a_button_hovered = true;
	}

	if (!is_a_button_hovered)
	{
		set_stat_strings_to_normal();
	}
}

void Tower::set_stat_strings_to_normal()
{
	auto dmg_value = std::to_string(int(mDamage.get_dmg_sum()));
	auto as_value = std::to_string(int(mAttack_speed));
	auto range_value = std::to_string(mRange);
	auto dmg_distribution_text = "P: " + std::to_string(int(mDamage.get_phys_dmg()))
		+ " M: " + std::to_string(int(mDamage.get_magic_dmg()))
		+ " F: " + std::to_string(int(mDamage.get_fire_dmg()))
		+ " W: " + std::to_string(int(mDamage.get_water_dmg()))
		+ " E: " + std::to_string(int(mDamage.get_elec_dmg()));
	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mDamage_distribution_text->set_text(dmg_distribution_text);
}

void Tower::set_stat_strings_for_upgrade_buttons(Button* button)
{
	//updates texture: stat-values for tower
	auto dmg_value = std::to_string(int(mDamage.get_dmg_sum()));
	auto as_value = std::to_string(int(mAttack_speed));
	auto range_value = std::to_string(int(mRange));

	auto tower_upgrade_section = mName + "/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	if (dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Damage" || dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Attackspeed" || dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Range")
	{
		tower_upgrade_section = "Tower/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	}

	dmg_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "magic")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "fire")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "water")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
	as_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed"));
	range_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "range"));
	auto dmg_distribution_text = "P: " + std::to_string(int(mDamage.get_phys_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys"))
		+ " M: " + std::to_string(int(mDamage.get_magic_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "magic"))
		+ " F: " + std::to_string(int(mDamage.get_fire_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "fire"))
		+ " W: " + std::to_string(int(mDamage.get_water_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "water"))
		+ " E: " + std::to_string(int(mDamage.get_elec_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "elec"));

	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mDamage_distribution_text->set_text(dmg_distribution_text);
}

void Tower::on_button_press(const int button_id, Button* button)
{
	if (button_id == UPGRADE_DAMAGE_BUTTON) upgrade_damage();
	if (button_id == UPGRADE_RANGE_BUTTON) upgrade_range();
	if (button_id == UPGRADE_ATTACKSPEED_BUTTON) upgrade_attack_speed();
	Building::on_button_press(button_id, button);
}

void Tower::set_clicked(const bool value)
{
	//break;
	Clickable::set_clicked(value);
}

void Tower::on_tick()
{
	Building::on_tick();
	// try to shoot
	const auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
	if (mElapsed_ticks % mAttack_cooldown == 0 && !this->mIdle)
	{
		const auto end = all_enemies->end();
		for (auto it = all_enemies->begin(); it != end; ++it)
		{
			const auto enemy = dynamic_cast<Enemy*>(*it);
			if (enemy_in_range(enemy, mRange, mCoords) && !enemy->is_dead())
			{
				this->create_shot(enemy);
				break;
			}
		}
	}
}

void Tower::on_click(int mouse_x, int mouse_y)
{
	SDL_Rect rect;
	rect.x = 1280;
	rect.y = 600;
	rect.w = 600;
	rect.h = 200;
	mLevel->get_menu()->set_building_window(mBuilding_window);
	Building::on_click(mouse_x, mouse_y);
}

bool Tower::upgrade(const std::string& tower_upgrade_section)
{
	if(Building::upgrade(tower_upgrade_section))
	{
		mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
			gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
			gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
			gConfig_file->value_or_zero(tower_upgrade_section, "water"),
			gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
		mRange += gConfig_file->value_or_zero(tower_upgrade_section, "range");
		mAttack_speed += gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed");
		mProjectile_speed += gConfig_file->value_or_zero(tower_upgrade_section, "projectilespeed");
		mProjectile_name.assign(gConfig_file->value(tower_upgrade_section, "projectile_name"));
		mAttack_cooldown = int(*gFrame_rate / mAttack_speed);
		return true;
	}
	return false;
}

bool Tower::upgrade_damage()
{
	const auto tower_upgrade_section = "Tower/upgradeDamage";
	if(Building::upgrade(tower_upgrade_section))
	{
		mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
			gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
			gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
			gConfig_file->value_or_zero(tower_upgrade_section, "water"),
			gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
		mCount_of_little_upgrades++;
		mNumber_of_damage_upgrades++;
		return true;
	}
	return false;
}

bool Tower::upgrade_range()
{
	const auto tower_upgrade_section = "Tower/upgradeRange";
	if(Building::upgrade(tower_upgrade_section))
	{
		mRange += gConfig_file->value(tower_upgrade_section, "range");
		mCount_of_little_upgrades++;
		mNumber_of_range_upgrades++;
		return true;
	}
	return false;
}

bool Tower::upgrade_attack_speed()
{
	const auto tower_upgrade_section = "Tower/upgradeAttackspeed";
	if(Building::upgrade(tower_upgrade_section))
	{
		mAttack_speed += gConfig_file->value(tower_upgrade_section, "attackspeed");
		mAttack_cooldown = int(*gFrame_rate / mAttack_speed);
		mCount_of_little_upgrades++;
		mNumber_of_attackspeed_upgrades++;
		return true;
	}
	return false;
}

bool Tower::enemy_in_range(Enemy* enemy, const double radius, const SDL_Point center)
{
	const auto x_div = center.x - enemy->get_position().x;
	const auto y_div = center.y - enemy->get_position().y;
	const auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	return dist_to_enemy <= radius;
}

std::string Tower::get_projectile_name() const
{
	return mProjectile_name;
}

double Tower::get_projectile_speed() const
{
	return this->mProjectile_speed;
}

Damage Tower::get_damage() const
{
	return this->mDamage;
}

BUILDINGTYPE Tower::get_building_type()
{
	return BUILDINGTYPE::TOWER;
}

double Tower::get_attack_speed() const
{
	return mAttack_speed;
}

double Tower::get_range() const
{
	return mRange;
}

int Tower::get_number_of_damage_upgrades() const
{
	return mNumber_of_damage_upgrades;
}

int Tower::get_number_of_attackspeed_upgrades() const
{
	return mNumber_of_attackspeed_upgrades;
}

int Tower::get_number_of_range_upgrades() const
{
	return mNumber_of_range_upgrades;
}

void Tower::increment_number_of_damage_upgrades()
{
	mNumber_of_damage_upgrades++;
}

void Tower::increment_number_of_attackspeed_upgrades()
{
	mNumber_of_attackspeed_upgrades++;
}

void Tower::increment_number_of_range_upgrades()
{
	mNumber_of_range_upgrades++;
}

