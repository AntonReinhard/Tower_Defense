#pragma once
#include "Damage.h"
#include "Constants.h"
#include <string>

/*
 * Holds all the defensive stats of a Unit: life, armor, resistances, immunities
 * Also able to directly calculate damage taken when given a Damage Object
 */
class Defense
{
public:
    Defense();
    Defense(double health, double armor, double magic_res, double fire_res, double water_res, double elec_res);

    //Setting all the different defenses
    void set_resistances(double armor, double magic_res, double fire_res, double water_res, double elec_res);
    void set_resistances(RESISTANCES type, int value);
    double get_resistance(RESISTANCES type);
    void set_immunities(bool phys, bool magic, bool fire, bool water, bool elec);
    void set_health(double health);
    double get_health() const;
    void set_full_health(double health);
    double get_full_health() const;

    //Calculating the actual Damage taken from an object of the Damage type
    //Returns true if unit got killed
    bool take_damage(const Damage& dmg);

    static std::string get_name(RESISTANCES type);

    Defense get_display_defenses();

private:
    double mHealth;
    double mFull_health;

    double mResistances[RESISTANCES_TOTAL];
    bool mImmunities[RESISTANCES_TOTAL];
    double mDisplay[RESISTANCES_TOTAL];

    void set_display_zero();
};
