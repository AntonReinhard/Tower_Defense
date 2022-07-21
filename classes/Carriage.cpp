#include "Carriage.h"
#include "Unit.h"
#include "SDL_setup.h"
#include <set>
#include "ConfigFile.h"
#include "WareHouse.h"
#include "Path.h"

Carriage::Carriage(const std::string& unit_name, Level* level, LAYERS render_layer, Building* source, Building* drain) 
    : Unit{unit_name, level, render_layer}
    , mSource(source)
    , mDrain(drain)
    , mDelivering(*drain)
    , mRemoving(*source)
    , mCurrent_activity(GETTING_IDLE)
    , mLevel(level)
{
    if (mSource != nullptr)
    {
        mPosition.x = mSource->get_coords().x + TILE_WIDTH / 2.;
        mPosition.y = mSource->get_coords().y + TILE_HEIGHT / 2.;
    }

    const auto section = unit_name + "/stats";

    mCurrent_resources.read_from_config(section, nullptr, "%scapacity");

    update_transportation();
}

Carriage::~Carriage()
{
    // TODO: make the carriage return first?
    //put the resources left in the carriage back into the warehouse
    if (mLevel->get_main_building() != nullptr)
        mLevel->get_main_building()->add_resources(mCurrent_resources);
}

void Carriage::on_tick()
{
    this->move();
}

void Carriage::set_source(Building* b)
{
    this->mSource = b;
    this->mRemoving.update(*mSource);
    update_transportation();
}

void Carriage::set_drain(Building* b)
{
    this->mDrain = b;
    this->mDelivering.update(*mDrain);
    update_transportation();
}

Building* Carriage::get_source() const
{
    return this->mSource;
}

Building* Carriage::get_drain() const
{
    return this->mDrain;
}

void Carriage::update_transportation()
{
    for (auto i = 0; i < RESOURCES_TOTAL; i++)
    {
        auto ir = RESOURCETYPES(i);
        if (mDelivering[ir] != NONE)
            mTransporting[ir] = mDelivering[ir];
        else if (mRemoving[ir] == CONSUMING)
            mTransporting[ir] = PRODUCING;
        else if (mRemoving[ir] == PRODUCING)
            mTransporting[ir] = CONSUMING;
        else
            mTransporting[ir] = NONE;
    }
}

bool Carriage::move_towards(const SDL_Point target)
{
    //building under the carriage (probably a path)
    const auto here = mLevel->get_building_matrix(int(mPosition.x / TILE_WIDTH), int(mPosition.y / TILE_WIDTH));

    const auto speed_multiplier = (here == nullptr) ? 1. : (here->get_building_type() == STREET) ? dynamic_cast<Path*>(here)->get_speed_multiplier() : 1.;

    const auto move_dist = mMove_speed / 60.0 * speed_multiplier;

    const auto x_d = int(target.x - mPosition.x);
    const auto x_d_abs = abs(x_d); //take the absolute value for further calculations
    const auto y_d = int(target.y - mPosition.y);
    const auto y_d_abs = abs(y_d); //same as above

    const auto distance_to_target = sqrt(x_d * x_d + y_d * y_d);

    //set direction for rendering
    if (x_d > 0)
        mDirection = RIGHT;
    else if (x_d < 0)
        mDirection = LEFT;
    if (y_d > 0)
        mDirection = DOWN;
    else if (y_d < 0)
        mDirection = UP;

    if (move_dist > distance_to_target)
    {
        this->mPosition.x = target.x;
        this->mPosition.y = target.y;
        return true;
    }

    mPosition.x += move_dist * (double(x_d) / (x_d_abs + y_d_abs));
    mPosition.y += move_dist * (double(y_d) / (x_d_abs + y_d_abs));

    return false;
}

void Carriage::move()
{
    const auto x_tile = static_cast<int>(this->mPosition.x / TILE_WIDTH);
    const auto y_tile = static_cast<int>(this->mPosition.y / TILE_HEIGHT);
    const auto here = mLevel->get_building_matrix(x_tile, y_tile);

    //getting the building that's at the next checkpoint for later use
    Building* next_building = nullptr;
    if (!mCheckpoints.empty())
        next_building = mLevel->get_building_matrix(mCheckpoints[0].x / TILE_WIDTH, mCheckpoints[0].y / TILE_HEIGHT);

    switch (this->mCurrent_activity)
    {
    case GETTING_IDLE:		
        if (update_checkpoints_to(here, mSource))
        {
            this->mCurrent_activity = GETTING;
        }
        break;
    case GETTING:
        if (mCheckpoints.empty())
        {
            //carriage arrived
            this->mSource->transfer_resources(mCurrent_resources, mTransporting, true);
            this->mCurrent_activity = DELIVERING_IDLE;
            break;
        }

        //check if there's still a path where we're going (and we're not going to the last building, which is never a path)
        if (next_building == nullptr 
            || (next_building->get_building_type() != STREET
            && mCheckpoints.size() >= 2))
        {
            //if not, recalculate path
            update_checkpoints_to(here, mSource);
            break;
        }

        if (this->move_towards(mCheckpoints[0]))
        {
            //delete first checkpoint
            mCheckpoints.erase(mCheckpoints.begin());
        }
        break;
    case DELIVERING_IDLE:
        if (update_checkpoints_to(here, mDrain))
        {
            this->mCurrent_activity = DELIVERING;
        }
        break;
    case DELIVERING:
        if (mCheckpoints.empty())
        {
            //carriage arrived
            this->mDrain->transfer_resources(mCurrent_resources, mTransporting, false);
            this->mCurrent_activity = GETTING_IDLE;
            break;
        }

        //check if there's still a path where we're going (and we're not going to the last building, which is never a path)
        if (next_building == nullptr
            || (next_building->get_building_type() != STREET
            && mCheckpoints.size() >= 2))
        {
            //if not, recalculate path
            update_checkpoints_to(here, mDrain);
            break;
        }

        if (this->move_towards(mCheckpoints[0]))
        {
            //delete first checkpoint
            mCheckpoints.erase(mCheckpoints.begin());
        }
        break;
    default:
        break;
    }
}

//if path exists returns true and sets the path into mCheckpoints, otherwise returns false
bool Carriage::update_checkpoints_to(Building * source, Building * target)
{
    mCheckpoints.clear();

    if (source == nullptr || target == nullptr) return false;

    //if already there
    if (source == target) return true;

    //breadth-first search because there isn't enough space for difficult searches
    //and we find the shortest path guaranteed
    std::map<int, std::vector<std::pair<Building*, Building*>>> depth_search;
    //what buildings have already been visited
    std::set<Building*> visited;
    //		depth, vector of buildings in depth with mapped precursor
    depth_search[0].emplace_back(std::pair<Building*, Building*>(target, nullptr));
    visited.insert(target);

    auto success = false;
    auto distance = 0;

    for (auto depth = 0; !success; depth++) {	//going "up" through the depths
        //break if there's nothing more to search
        if (depth_search[depth].empty()) break;
        for (auto it = depth_search[depth].begin(); it != depth_search[depth].end(); ++it) {	//iterating through each depth

            auto neighbors = it->first->get_neighbors();

            for (auto neighbor : neighbors) { //iterating through 
                if (visited.find(neighbor) == visited.end() &&
                    (neighbor->get_building_type() == STREET || neighbor == source))
                {
                    //if this new building hasn't been visited
                    //and is either a street or the target
                    depth_search[depth + 1].emplace_back(std::pair<Building*, Building*>(neighbor, it->first));
                    visited.insert(neighbor);
                    if (neighbor == source)
                    {
                        success = true;
                        distance = depth + 1;
                    }
                }
            }
        }
    }
    // target not found
    if (distance == 0) return false;

    //collect path from source to target
    auto current = source;
    for (auto depth = distance; depth > 0; depth--)
    {
        for (auto& it : depth_search[depth])
        {
            if (it.first == current)
            {
                //add to checkpoints
                auto p = it.second->get_coords();
                p.x += TILE_WIDTH / 2;
                p.y += TILE_HEIGHT / 2;
                mCheckpoints.emplace_back(p);
                current = it.second;
            }
        }
    }
    return true;
}
