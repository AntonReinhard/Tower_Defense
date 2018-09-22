#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include <SDL.h>
#include "EntityHandler.h"


Tower::Tower(std::string tower_name, SDL_Point coords, Level *level) : Building(tower_name, coords, level)
{
	mTower_name = tower_name;

	mDamage.set_damages(gConfig_file->Value(mTower_name + "/stats", "phys"),
		gConfig_file->Value(mTower_name + "/stats", "magic"),
		gConfig_file->Value(mTower_name + "/stats", "fire"),
		gConfig_file->Value(mTower_name + "/stats", "water"),
		gConfig_file->Value(mTower_name + "/stats", "elec"));

	mRange = gConfig_file->Value(mTower_name + "/stats", "range");
	mAttack_speed = gConfig_file->Value(mTower_name + "/stats", "attackspeed");
	mProjectile_speed = gConfig_file->Value(mTower_name + "/stats", "projectilespeed");
	mProjectile_name.assign(gConfig_file->Value(mTower_name + "/stats", "projectile_name"));
	mAttack_cooldown = 60 / mAttack_speed;

}

Tower::~Tower()
{
	for (auto i = 0; i<mShots.size(); i++)
	{
		delete mShots.at(i);
	}
	mShots.clear();
}

ENTITYTYPE Tower::get_type()
{
	return ENTITYTYPE::TOWER;
}

void Tower::render()
{
	Building::render();
	for (auto i = 0; i < mShots.size(); i++)
	{
		this->render_shot(mShots.at(i));
	}
}

void Tower::on_tick()
{
	Building::on_tick();
	// try to shoot
	auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
	if (mElapsed_ticks % mAttack_cooldown == 0)
	{
		if(!this->mIdle)
		{
			auto end = all_enemies->end();
			for (auto it = all_enemies->begin(); it != end; ++it)
			{
				if (enemy_in_range(dynamic_cast<Enemy*>(*it), mRange, mCoords))
				{
					mShots.push_back(this->create_shot(dynamic_cast<Enemy*>(*it)));
					break;
				}
			}
		}
	}
	
	for (auto i = 0; i<mShots.size(); i++)
	{
		if(this->update_shot(mShots.at(i)))
		{
			mShots.erase(mShots.begin() + i);
			--i;
		}
	}
}

bool Tower::enemy_in_range(Enemy* enemy, double radius, SDL_Point center) const
{
	auto x_div = center.x - enemy->get_position().x;
	auto y_div = center.y - enemy->get_position().y;
	auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	if(dist_to_enemy <= radius)
	{
		return true;
	}
	return false;
}



std::string Tower::get_projectile_name() const
{
	return mProjectile_name;
}

double Tower::get_projectile_speed() const
{
	return mProjectile_speed;
}

