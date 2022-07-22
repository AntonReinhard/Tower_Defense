#include "Defense.h"

Defense::Defense() : mResistances{ 0. }, mImmunities{ false }, mDisplay{ 0. }
{
    mFull_health = 0.0;
    mHealth = 0.0;

    set_display_zero();
}

Defense::Defense(const double health, const double armor, const double magic_res, const double fire_res,
    const double water_res, const double elec_res) : mResistances{ 0. }, mImmunities{ false }, mDisplay{ 0. }
{
    mFull_health = health;
    mHealth = health;

    mResistances[ARMOR] = armor;
    mResistances[MAGIC_RES] = magic_res;
    mResistances[FIRE_RES] = fire_res;
    mResistances[WATER_RES] = water_res;
    mResistances[ELEC_RES] = elec_res;

    set_display_zero();
}

void Defense::set_resistances(const double armor, const double magic_res, const double fire_res,
                           const double water_res, const double elec_res)
{
    mResistances[ARMOR] = armor;
    mResistances[MAGIC_RES] = magic_res;
    mResistances[FIRE_RES] = fire_res;
    mResistances[WATER_RES] = water_res;
    mResistances[ELEC_RES] = elec_res;
}

void Defense::set_resistances(const RESISTANCES type, const int value)
{
    mResistances[type] = value;
}

double Defense::get_resistance(const RESISTANCES type)
{
    return mResistances[type];
}

void Defense::set_immunities(const bool phys, const bool magic, const bool fire, const bool water, const bool elec)
{
    mImmunities[ARMOR] = phys;
    mImmunities[MAGIC_RES] = magic;
    mImmunities[FIRE_RES] = fire;
    mImmunities[WATER_RES] = water;
    mImmunities[ELEC_RES] = elec;
}

void Defense::set_health(const double health)
{
    mHealth = health;
}

double Defense::get_health() const
{
    return mHealth;
}

void Defense::set_full_health(const double health)
{
    mFull_health = health;
}

double Defense::get_full_health() const
{
    return mFull_health;
}

bool Defense::take_damage(const Damage& dmg)
{
    auto phys_dmg = dmg.get_phys_dmg() * 100 / (100 + mResistances[ARMOR]);
    if (mImmunities[ARMOR]) phys_dmg = 0.0;

    auto magic_dmg = dmg.get_magic_dmg() * 100 / (100 + mResistances[MAGIC_RES]);
    if (mImmunities[MAGIC_RES]) magic_dmg = 0.0;

    auto fire_dmg = dmg.get_fire_dmg() * 100 / (100 + mResistances[FIRE_RES]);
    if (mImmunities[FIRE_RES]) fire_dmg = 0.0;

    auto water_dmg = dmg.get_water_dmg() * 100 / (100 + mResistances[WATER_RES]);
    if (mImmunities[WATER_RES]) water_dmg = 0.0;

    auto elec_dmg = dmg.get_elec_dmg() * 100 / (100 + mResistances[ELEC_RES]);
    if (mImmunities[ELEC_RES]) elec_dmg = 0.0;

    const auto total_dmg = phys_dmg + magic_dmg + fire_dmg + water_dmg + elec_dmg;

    auto killed = false;

    if (total_dmg >= mHealth)
    {
        mHealth = 0.0;
        killed = true;
    }
    else
    {
        mHealth -= total_dmg;
    }
    return killed;
}

std::string Defense::get_name(const RESISTANCES type)
{
    switch (type)
    {
    case ARMOR:
        return "Armor";
    case MAGIC_RES:
        return "Magic Resist";
    case FIRE_RES:
        return "Fire Resist";
    case WATER_RES:
        return "Water Resist";
    case ELEC_RES:
        return "Elec Resist";
    default:
        return "Unknown Resistance";
    }
}

Defense Defense::get_display_defenses()
{
    Defense def;

    for(auto i = 0; i < RESISTANCES_TOTAL; i++)
    {
        mDisplay[i] += float(mResistances[i] - mDisplay[i]) / 100.f;
        if (mResistances[i] - mDisplay[i] < 1.f) mDisplay[i] = mResistances[i];
        def.set_resistances(RESISTANCES(i), int(mDisplay[i]));
    }
    return def;
}

void Defense::set_display_zero()
{
    for (auto i = 0; i < RESISTANCES_TOTAL; i++)
    {
        mDisplay[RESISTANCES(i)] = 0;
    }
}
