#include "EntityHandler.h"
#include "Entity.h"

EntityHandler* gEntity_handler = nullptr;

EntityHandler::EntityHandler()
{
    //initialize mEntities_by_type and set all dirtybits to true
    for (auto i = 0; i < int(ENTITYTYPES_TOTAL); i++) {
        this->mEntities_by_type[ENTITYTYPE(i)] = new std::vector<Entity*>();
        this->mEntity_vector_dirty_bit[ENTITYTYPE(i)] = true;
    }
}

EntityHandler::~EntityHandler()
{
    mEntities.clear();
}

void EntityHandler::add_entity(Entity* u)
{
    this->invalidate_entity_vectors();
    this->mEntities.push_back(u);
}

void EntityHandler::del_entity(Entity* u)
{
    this->invalidate_entity_vectors();
    for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
    {
        if(*it == u)
        {
            mEntities.erase(it);
            break;
        }
    }
}

void EntityHandler::delete_all_entities()
{
    //do it this way because some entities also delete others with them
    while (!mEntities.empty())
    {
        delete mEntities.at(0);
    }
    mEntities.clear();
}

void EntityHandler::update()
{
    //can't use an iterator here because the particles will sometimes suicide and invalidate the iterator
    for (int i = this->mEntities.size() - 1; i >= 0; --i) {
        mEntities.at(i)->on_tick();
    }
}

const std::vector<Entity*>* EntityHandler::get_entities_of_type(const ENTITYTYPE type)
{
    if (!this->mEntity_vector_dirty_bit[type]) {
        return this->mEntities_by_type[type];
    }

    //if dirtybit was set, delete old vector here
    delete this->mEntities_by_type[type];

    auto v = new std::vector<Entity*>();

    for (auto const& it : mEntities)
    {
        if (it->get_type() == type)
        {
            v->push_back(it);
        }
    }

    //save the updated vector and set dirtybit back to false
    this->mEntities_by_type[type] = v;
    this->mEntity_vector_dirty_bit[type] = false;

    return v;
}

void EntityHandler::invalidate_entity_vectors()
{
    for (auto i = 0; i < int(ENTITYTYPE::ENTITYTYPES_TOTAL); i++) {
        this->mEntity_vector_dirty_bit[ENTITYTYPE(i)] = true;
    }
}
