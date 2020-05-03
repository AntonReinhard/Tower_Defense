#pragma once
#include <string>

#include "Unit.h"
#include "Resources.h"
#include "HomingShot.h"


class Level;

class Enemy final :
	public Unit
{
public:
	Enemy(const std::string& monster_name, int way, Level* level, LAYERS render_layer);
	~Enemy();

	void on_tick() override;
	void on_click(int mouse_x, int mouse_y) override;
	
	//moves the enemy along it's assigned path
	//if it hits the last checkpoint it calls got_through()
	//and sets itself to dead
	void move();
	void got_through();
	SDL_Point get_position() const;
	bool take_damage(Damage *dmg);
	//is called once on death
	void on_death();
	void render() override; //calls Unit::render() and shows the health bar with two rectangles

	void add_following_shot(HomingShot* shot);

	bool is_dead() const;
	ENTITYTYPE get_type() override;

	std::vector<SDL_Point> get_checkpoints() const;
	Vector get_move_diretion() const;
	void update_move_direction(Vector old_position);
private:
	std::vector<SDL_Point> mCheckpoints{};
	bool mDead;
	//how much life will be lost when the enemy gets through
	int mLife_cost;
	Resources mLoot_resources;
	
	//TODO: make healthbar its own class? not sure
	LTexture* mFull_health_bar;
	LTexture* mEmpty_health_bar;
	SDL_Rect mHealth_bar_dimensions;
	std::vector<HomingShot*> mFollowed_by;
	Vector mMove_direction; //unit vector for the move direction
};
