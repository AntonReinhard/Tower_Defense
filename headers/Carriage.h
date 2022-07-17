#pragma once
#include "Unit.h"
#include "Building.h"
#include "Resources.h"
#include "Production.h"

class Carriage final
    : public Unit
{
public:
    explicit Carriage(const std::string& unit_name, Level* level, LAYERS render_layer, Building* source = nullptr, Building* drain = nullptr);
    ~Carriage();

    void on_tick() override;

    void set_source(Building* b);
    void set_drain(Building* b);

    Building* get_source() const;
    Building* get_drain() const;

    void update_transportation();

    bool move_towards(SDL_Point target);

    void move();

    bool update_checkpoints_to(Building* source, Building* target);

private:
    //building the carriage is getting resources from
    Building* mSource;
    //building the carriage is bringing resources to
    Building* mDrain;

    //total capacity
    Resources mCurrent_resources;

    //what will be delivered to the drain, saved as what the drain is consuming/producing
    Production mDelivering;

    //what will be removed from the source, saved as what the source is consuming/producing
    Production mRemoving;

    //what will be transported, saved from the drain's perspective
    Production mTransporting;

    ACTIVITY mCurrent_activity;

    std::vector<SDL_Point> mCheckpoints;

    Level* mLevel;
};
