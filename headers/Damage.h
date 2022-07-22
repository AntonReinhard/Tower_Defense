#pragma once

class Damage
{
public:
    Damage();
    Damage(double phys, double magic, double fire, double water, double elec);
    Damage(int phys, int magic, int fire, int water, int elec);

    void add(double phys, double magic, double fire, double water, double elec);
    //set and get all the different types of damage
    void set_damages(double phys, double magic, double fire, double water, double elec);
    void set_phys_dmg(double phys);
    double get_phys_dmg() const;
    void set_magic_dmg(double magic);
    double get_magic_dmg() const;
    void set_fire_dmg(double fire);
    double get_fire_dmg() const;
    void set_water_dmg(double water);
    double get_water_dmg() const;
    void set_elec_dmg(double elec);
    double get_elec_dmg() const;

    double get_dmg_sum() const;

private:
    double mPhys_dmg;
    double mMagic_dmg;
    double mFire_dmg;
    double mWater_dmg;
    double mElec_dmg;
};
