#include "Entity.h"
#include "EntityHandler.h"

Entity::Entity(LAYERS render_layer) : Renderable(render_layer)
{
    gEntity_handler->add_entity(this);
}

Entity::~Entity()
{
    gEntity_handler->del_entity(this);
}

ENTITYTYPE Entity::get_type()
{
    return ENTITYTYPE::UNDEFINED;
}
