#pragma once
#include "Entity.h"
#include <vector>
#include <map>

class EntityHandler
{
public:
    EntityHandler();
    ~EntityHandler();

    void add_entity(Entity* u);
    void del_entity(Entity* u);

    void delete_all_entities();

    void update();

    const std::vector<Entity*>* get_entities_of_type(ENTITYTYPE type);

private:
    void invalidate_entity_vectors();

    std::vector<Entity*> mEntities;

    std::map<ENTITYTYPE, std::vector<Entity*>*> mEntities_by_type;

    std::map<ENTITYTYPE, bool> mEntity_vector_dirty_bit;
};

extern EntityHandler* gEntity_handler;
