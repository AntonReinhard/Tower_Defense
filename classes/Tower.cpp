#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "Shot.h"
#include <iostream>


Tower::Tower(std::string tower_name, SDL_Point coords) : Building(tower_name, coords)
{
	ConfigFile cf("config/game.cfg");
	mTower_name = tower_name;
	mDamage.set_damages(cf.Value(tower_name + "/stats", "phys"),
		cf.Value(tower_name + "/stats", "magic"),
		cf.Value(tower_name + "/stats", "fire"),
		cf.Value(tower_name + "/stats", "water"),
		cf.Value(tower_name + "/stats", "elec"));

	mRange = cf.Value(tower_name + "/stats", "range");
	mAS = cf.Value(tower_name + "/stats", "attackspeed");
	mProjectile_speed = cf.Value(tower_name + "/stats", "projectilespeed");
	mProjectile_name.assign(cf.Value(tower_name + "/stats", "projectile_name"));
	mAttack_cooldown = 60 / mAS;
	mElapsed_ticks = 0;
}

Tower::~Tower()
{
}

void Tower::render()
{
	Building::render();
	for(auto i = 0; i<mShots.size(); i++)
	{
		mShots.at(i)->render();
	}
}

void Tower::update(std::vector<Enemy*> all_enemies)
{
	if(mElapsed_ticks == 0)
	{
		while (!all_enemies.empty())
		{
			if (enemy_in_range(all_enemies.at(0)))
			{
				
				auto shot = new Shot(mProjectile_name, *this, mProjectile_speed, all_enemies.at(0));
				std::cout << "new shot" << std::endl;
				mShots.push_back(shot);
			}
			all_enemies.erase(all_enemies.begin());
		}
		this->shot();
		return;
	}
	mElapsed_ticks--;
}

bool Tower::enemy_in_range(Enemy* enemy)
{
	auto x_div = mCoords.x - enemy->get_position().x;
	auto y_div = mCoords.y - enemy->get_position().y;
	auto dist_to_enemy = x_div * x_div + y_div * y_div;
	dist_to_enemy = sqrt(dist_to_enemy);
	if(dist_to_enemy <= mRange)
	{
		return true;
	}
	return false;
}

//all projectiles, that are fired from this tower are updated
void Tower::shot()
{ 
	mElapsed_ticks = mAttack_cooldown;
	for(auto i=0; i<mShots.size(); i++ )
	{
		std::cout << mShots[i] << std::endl;
		if (mShots[i]->follow())
		{
			std::cout << "Health: " << mShots[i]->get_enemy_to_shot().get_defense().get_health();
			mShots[i]->get_enemy_to_shot().get_defense().take_damage(&mDamage);
			std::cout << "Health: " << mShots[i]->get_enemy_to_shot().get_defense().get_health();
			delete mShots[i];
			mShots.erase(mShots.begin() + i);
		}
	}
}




