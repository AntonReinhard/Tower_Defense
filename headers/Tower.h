#pragma once
#include <SDL.h>

#include "Building.h"
#include "Damage.h"
#include "Enemy.h"

class Tower : 
	public Building
{
public:
	Tower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);
	~Tower();

	virtual void create_shot(Enemy* enemy) = 0;

	//calls Building::render and renders all shots from the tower
	void render() override;
	void set_clicked(const bool value) override;
	
	
	//all projectiles, that are fired from this tower are updated, shots are fired
	void on_tick() override;
	void on_click(int mouse_x, int mouse_y) override;

	bool upgrade(const std::string& tower_upgrade_section) override;
	bool upgrade_damage();
	bool upgrade_range();
	bool upgrade_attack_speed();

	double get_attack_speed() const;
	double get_range() const;
	double get_explosive_radius() const;
	int get_number_of_damage_upgrades() const;
	int get_number_of_attackspeed_upgrades() const;
	int get_number_of_range_upgrades() const;

	void increment_number_of_damage_upgrades();
	void increment_number_of_attackspeed_upgrades();
	void increment_number_of_range_upgrades();

	BUILDINGTYPE get_building_type() override;
	
	//checks if an enemy is in range of the tower
	static bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center);

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

	Damage get_damage() const;

	std::shared_ptr<Window> create_window() override;
	
	void update_building_window() override;
	//if an upgradebutton is hovered this function is called and sets the strings with the stats for the upgrade, that are shown in the window
	void set_stat_strings_for_upgrade_buttons(UpgradeButton* button) override;
	//sets strings in window so that they do not show the upgrade values of the last upgradebutton that was hovered

	void on_button_press(int button_id, Button* button) override;

	bool get_predicts() const;

protected:
	int mAttack_cooldown; //60 / mAttackspeed
	double mRange;
	double mAttack_speed; //Attacks per second
	double mProjectile_speed;
	double mExplosive_radius; // = 0 if single target, otherwise >= 0
	bool mPredict; // if true tower predicts the movement and shoots where the enemy will be
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;
	LTexture* mRange_indicator_sprite;
	int mNumber_of_damage_upgrades = 0;
	int mNumber_of_attackspeed_upgrades = 0;
	int mNumber_of_range_upgrades = 0;

	UpgradeButton* mUpgrade_damage_button;
	UpgradeButton* mUpgrade_range_button;
	UpgradeButton* mUpgrade_attackspeed_button;

	Text* mDamage_upgrade_number_texture;
	Text* mAttackspeed_upgrade_number_texture;
	Text* mRange_upgrade_number_texture;
	Text* mDmg_value;
	Text* mAs_value;
	Text* mRange_value;
	Text* mDamage_distribution_value;
	Text* mExplosive_radius_value;
};

