#pragma once
#include "MonsterGroup.h"

class Wave
{
public:
    Wave(const std::string& wave_number, Level* level);

    //at the moment: update the first monster group in the vector, if it is dead spawn the next wave
    void update();

    //Wave::update deletes mMonster_groups.at(0), if the monster group is dead, so the next monster group will be rendered
    bool is_dead() const;

    std::vector<MonsterGroup*>* get_monster_groups();

    int get_spawn_delay();
    int get_elapsed_ticks();

    std::string get_wave_number();

private:
    std::vector<MonsterGroup*> mMonster_groups;
    Level* mLevel;
    int mSpawn_delay;
    int mElapsed_ticks; //increments every tick as soon as the level started
    std::string mWave_number;
};
