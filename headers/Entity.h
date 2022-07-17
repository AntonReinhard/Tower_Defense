#pragma once
#include "Renderable.h"
#include "Enums.h"

class Entity :
    public Renderable
{
public:
    Entity(LAYERS render_layer);
    virtual ~Entity();

    virtual void on_tick() = 0;

    virtual ENTITYTYPE get_type();
};
