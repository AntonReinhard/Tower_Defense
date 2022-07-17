#include "Resources.h"

#include <algorithm>

Resources::Resources()
    : mResources{ 0 }
    , mDisplay{ 0.0 }
{
    this->set_empty();
    this->mLimit = nullptr;
    this->set_display_zero();
}

Resources::Resources(const double gold, const double wood, const double stone, const double iron, const double energy, const double water, const double food)
    : mResources { 0 }
    , mDisplay{ 0.0 }
{
    this->set_resources(gold, wood, stone, iron, energy, water, food);
    this->mLimit = nullptr;
    this->set_display_zero();
}

Resources::Resources(const int gold, const int wood, const int stone, const int iron, const int energy, const int water, const int food)
    : mResources{ 0 }
    , mDisplay{ 0.0 }
{
    this->set_resources(double(gold), double(wood), double(stone), double(iron), double(energy), double(water), double(food));
    this->mLimit = nullptr;
    this->set_display_zero();
}

Resources::Resources(const Resources& resource, std::optional<Resources> limit)
    : mResources{ 0 }
    , mDisplay{ 0.0 }
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        this->set_resource(RESOURCETYPES(i), resource.get_resource(RESOURCETYPES(i)));
    }
    if (limit.has_value()) {
        this->mLimit = std::make_unique<Resources>(limit.value());
    }
    else {
        this->mLimit = nullptr;
    }
    this->set_display_zero();
}

Resources::Resources(const Resources& r) {
    *this = r;
}

Resources& Resources::operator=(const Resources& r)
{
    if (this == &r) {
        return *this;
    }
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        this->set_resource(RESOURCETYPES(i), r.get_resource(RESOURCETYPES(i)));
    }
    if (r.has_limit())
    {
        this->mLimit = std::make_unique<Resources>(*r.mLimit);
    }
    return *this;
}


Resources::~Resources() = default;

void Resources::set_resources(const double gold, const double wood, const double stone, const double iron, const double energy, const double water, const double food)
{
    mResources[GOLD] = gold;
    mResources[FOOD] = food;
    mResources[WOOD] = wood;
    mResources[STONE] = stone;
    mResources[IRON] = iron;
    mResources[WATER] = water;
    mResources[ENERGY] = energy;
}

void Resources::set_resources(const int gold, const int wood, const int stone, const int iron, const int energy, const int water, const int food)
{
    mResources[GOLD] = double(gold);
    mResources[FOOD] = double(food);
    mResources[WOOD] = double(wood);
    mResources[STONE] = double(stone);
    mResources[IRON] = double(iron);
    mResources[WATER] = double(water);
    mResources[ENERGY] = double(energy);
}

void Resources::set_resource(const RESOURCETYPES type, const double res)
{
    this->mResources[type] = res;
}

const double& Resources::get_resource(const RESOURCETYPES type) const&
{
    return mResources[type];
}

double& Resources::get_resource(const RESOURCETYPES type) &
{
    return mResources[type];
}

void Resources::set_empty()
{
    for (auto& resource : mResources)
    {
        resource = 0;
    }
}

bool Resources::is_empty()
{
    for (auto resource : mResources)
    {
        if (resource != 0) return false;
    }
    return true;
}

void Resources::add(const RESOURCETYPES type, const double res)
{
    if (has_limit()) {
        if (res + mResources[type] > mLimit->get_resource(type)) {
            mResources[type] = mLimit->get_resource(type);
            return;
        }
    }
    mResources[type] += res;
}

bool Resources::sub(const RESOURCETYPES type, const double res)
{
    if (mResources[type] - res < 0) {
        return false;
    }
    mResources[type] -= res;
    return true;
}

//Returns true if the subtracting succeeded, false if there wasn't enough resources
bool Resources::sub(const Resources& cost)
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        if (mResources[i] - cost.get_resource(RESOURCETYPES(i)) < 0) {
            return false;
        }
    }
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        mResources[i] -= cost.get_resource(RESOURCETYPES(i));
    }
    return true;
}

bool Resources::sub_possible(const Resources& cost)
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        if (mResources[i] - cost.get_resource(RESOURCETYPES(i)) < 0) {
            return false;
        }
    }
    return true;
}


void Resources::add(const Resources& income)
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        this->add(RESOURCETYPES(i), income.get_resource(RESOURCETYPES(i)));
    }
}

Resources Resources::operator/(const double &d) const
{
    Resources r;

    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        r.set_resource(RESOURCETYPES(i), this->get_resource(RESOURCETYPES(i)) / d);
    }
    return r;
}

void Resources::set_limit(const Resources& limit)
{
    this->mLimit = std::make_unique<Resources>(limit);
}

const std::optional<Resources>& Resources::get_limit() const
{
    if (mLimit == nullptr) return {};
    return *mLimit;
}

bool Resources::has_limit() const 
{
    return mLimit != nullptr;
}

bool Resources::transfer(Resources& source)
{
    if (has_limit()) {
        this->add(source);
        source.set_empty();
        return true;
    }

    for (auto i = 0; i < RESOURCES_TOTAL; i++) 
    {
        auto adding = mLimit->get_resource(RESOURCETYPES(i)) - mResources[RESOURCETYPES(i)];
        //skip if over full already
        if (adding <= 0) continue;
        adding = std::min(adding, source.get_resource(RESOURCETYPES(i)));
        this->add(RESOURCETYPES(i), source.get_resource(RESOURCETYPES(i)));
        source.sub(RESOURCETYPES(i), adding);
    }

    return source.is_empty();
}

bool Resources::transfer(const RESOURCETYPES type, double& r)
{
    if (!has_limit())
    {
        this->add(type, r);
        r = 0;
        return true;
    }

    auto adding = mLimit->get_resource(type) - mResources[type];
    if (adding > 0)
    {
        adding = std::min(adding, r);
        this->add(type, r);
        r -= adding;
    }

    return r == 0;
}

Resources Resources::get_display_resources()
{
    Resources res;

    for (auto i = 0; i < RESOURCES_TOTAL; i++) {
        this->mDisplay[i] += double(mResources[i] - mDisplay[i]) / 100.f;
        if (mResources[i] - mDisplay[i] < 1.f) mDisplay[i] = double(mResources[i]);
        res.set_resource(RESOURCETYPES(i), int(mDisplay[i]));
    }
    return res;
}

std::string Resources::get_name(const RESOURCETYPES type)
{
    //TODO: locale stuff monkaS
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

void Resources::set_display_zero()
{
    for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
        this->mDisplay[RESOURCETYPES(i)] = 0;
    }
}
