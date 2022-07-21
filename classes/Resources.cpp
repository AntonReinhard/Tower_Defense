#include "Resources.h"
#include "ConfigFile.h"

#include <cmath>
#include <algorithm>
#include <tuple>
#include <array>
#include <ranges>
#include <iostream>

Resources::Resources() = default;

Resources::~Resources() = default;

// give name as "%sMain" to get goldMain, woodMain and so on
void Resources::read_from_config(const std::string& section, const char* name, const char* limitName) {
    char buf[255];

    if (name != nullptr) {
        for (auto i = 0; i < RESOURCES_TOTAL; ++i) {
            const auto res = static_cast<RESOURCETYPES>(i);
            sprintf(buf, name, Resources::to_string(res).c_str());
            set_resource(res, gConfig_file->value_or_zero(section, buf));
        }
    }

    if (limitName != nullptr) {
        for (auto i = 0; i < RESOURCES_TOTAL; ++i) {
            const auto res = static_cast<RESOURCETYPES>(i);
            sprintf(buf, limitName, Resources::to_string(res).c_str());
            set_limit(res, gConfig_file->value_or_zero(section, buf));
        }
    }
}

void Resources::set_resource(const RESOURCETYPES type, const double res)
{
    mResources[type].set(res);
}

double Resources::get_resource(const RESOURCETYPES type) const
{
    return mResources[type].get();
}

void Resources::set_limit(const RESOURCETYPES type, const double res)
{
    mResources[type].set_limit(res);
}

double Resources::get_limit(const RESOURCETYPES type) const
{
    return mResources[type].get_limit();
}

int Resources::get_display(const RESOURCETYPES type) const
{
    return mResources[type].get_display();
}

void Resources::set_empty()
{
    for (auto& resource : mResources)
    {
        resource.set(0.);
    }
}

bool Resources::empty()
{
    for (auto resource : mResources)
    {
        if (resource.empty()) return false;
    }
    return true;
}

//Returns true if the subtracting succeeded, false if there weren't enough resources
bool Resources::sub(const Resources& cost)
{
    if (!can_sub(cost)) {
        return false;
    }

    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = mResources[i];
        auto& c = cost.mResources[i];
        res.sub(c);
    }
    return true;
}

bool Resources::can_sub(const Resources& cost)
{
    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = mResources[i];
        auto& c = cost.mResources[i];
        if (!res.can_sub(c)) {
            return false;
        }
    }
    return true;
}

void Resources::add(const Resources& income)
{
    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = mResources[i];
        auto& in = income.mResources[i];

        res.add(in);
    }
}

Resources Resources::operator/(const double &d) const
{
    Resources r;

    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = r.mResources[i];
        res.set(mResources[i].get() / d);
    }
    return r;
}

void Resources::transfer(Resources& source)
{
    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = mResources[i];
        auto& in = source.mResources[i];

        res.transfer(in);
    }
}

void Resources::transfer(Resources& source, const Production& inout)
{
    for (auto i = 0; i < mResources.size(); i++) {
        auto& res = mResources[i];
        auto& in = source.mResources[i];
        const auto& prod = inout[i];

        res.transfer(in, prod);
    }
}

void Resources::transfer(const RESOURCETYPES type, Resources& r)
{
    auto i = static_cast<int>(type);
    return mResources[i].transfer(r.mResources[i]);
}

std::string Resources::to_string(const RESOURCETYPES type)
{
    switch (type) {
    case GOLD:
        return "gold";
    case WOOD:
        return "wood";
    case FOOD:
        return "food";
    case IRON:
        return "iron";
    case STONE:
        return "stone";
    case WATER:
        return "water";
    case ENERGY:
        return "energy";
    default:
        return "unknown resource";
    }
}

std::string Resources::get_name(RESOURCETYPES type)
{
    switch (type) {
    case GOLD:
        return "Gold";
    case WOOD:
        return "Wood";
    case FOOD:
        return "Food";
    case IRON:
        return "Iron";
    case STONE:
        return "Stone";
    case WATER:
        return "Water";
    case ENERGY:
        return "Energy";
    default:
        return "Unknown Resource";
    }
}

Resources::Res::Res() 
    : mRes(0)
    , mLimit(0)
    , mPrev_res(0)
    , mLast_change(std::chrono::system_clock::now())
{}

void Resources::Res::set(const double r)
{
    if (std::abs(mRes - r) > EPSILON) {
        mPrev_res = mRes;
        mRes = r;
        mLast_change = std::chrono::system_clock::now();
    }
}

double Resources::Res::get() const
{
    return mRes;
}

void Resources::Res::set_limit(const double limit)
{
    mLimit = limit;
    if (mRes > mLimit) {
        set(mLimit);
    }
}

double Resources::Res::get_limit() const
{
    return mLimit;
}

int Resources::Res::get_display() const
{
    return std::lround(mRes + std::pow(1.05, static_cast<double>((std::chrono::system_clock::now() - mLast_change).count() * -0.00001)) * (mPrev_res - mRes));
}

bool Resources::Res::empty() const
{
    return std::abs(mRes) < EPSILON;
}

void Resources::Res::add(const Res& r)
{
    set(std::min(get() + r.get(), mLimit));
}

bool Resources::Res::can_sub(const Res& r) const
{
    return get() - r.get() >= 0;
}

void Resources::Res::sub(const Res& r)
{
    set(std::max(get() - r.get(), 0.));
}

void Resources::Res::transfer(Res& source)
{
    double transfer_amount = std::min(get() + source.get(), mLimit) - get();

    set(get() + transfer_amount);
    source.set(source.get() - transfer_amount);
}

void Resources::Res::transfer(Res& source, const PRODUCTIONSTATE inout)
{
    switch (inout) {
    case PRODUCING:
        transfer(source);
        break;
    case CONSUMING:
        source.transfer(*this);
        break;
    }
}
