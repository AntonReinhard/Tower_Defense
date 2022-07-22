#include "Damage.h"

Damage::Damage()
{
    mPhys_dmg = 0.0;
    mMagic_dmg = 0.0;
    mFire_dmg = 0.0;
    mWater_dmg = 0.0;
    mElec_dmg = 0.0;
}

Damage::Damage(const double phys, const double magic, const double fire, const double water, const double elec)
{
    mPhys_dmg = phys;
    mMagic_dmg = magic;
    mFire_dmg = fire;
    mWater_dmg = water;
    mElec_dmg = elec;
}

Damage::Damage(const int phys, const int magic, const int fire, const int water, const int elec)
{
    mPhys_dmg = double(phys);
    mMagic_dmg = double(magic);
    mFire_dmg = double(fire);
    mWater_dmg = double(water);
    mElec_dmg = double(elec);
}

void Damage::add(const double phys, const double magic, const double fire, const double water, const double elec)
{
    mPhys_dmg += phys;
    mMagic_dmg += magic;
    mFire_dmg += fire;
    mWater_dmg += water;
    mElec_dmg += elec;
}


void Damage::set_damages(const double phys, const double magic, const double fire, const double water, const double elec)
{
    mPhys_dmg = phys;
    mMagic_dmg = magic;
    mFire_dmg = fire;
    mWater_dmg = water;
    mElec_dmg = elec;
}

void Damage::set_phys_dmg(const double phys)
{
    mPhys_dmg = phys;
}

double Damage::get_phys_dmg() const
{
    return mPhys_dmg;
}

void Damage::set_magic_dmg(const double magic)
{
    mMagic_dmg = magic;
}

double Damage::get_magic_dmg() const
{
    return mMagic_dmg;
}

void Damage::set_fire_dmg(const double fire)
{
    mFire_dmg = fire;
}

double Damage::get_fire_dmg() const
{
    return mFire_dmg;
}

void Damage::set_water_dmg(const double water)
{
    mWater_dmg = water;
}

double Damage::get_water_dmg() const
{
    return mWater_dmg;
}

void Damage::set_elec_dmg(const double elec)
{
    mElec_dmg = elec;
}

double Damage::get_elec_dmg() const
{
    return mElec_dmg;
}

double Damage::get_dmg_sum() const
{
    return mPhys_dmg + mMagic_dmg + mFire_dmg + mWater_dmg + mElec_dmg;
}
