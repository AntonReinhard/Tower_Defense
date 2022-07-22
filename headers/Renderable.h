#pragma once
#include "Enums.h"

class Renderable
{
public:
    Renderable(LAYERS render_layer);
    virtual ~Renderable();

    virtual void render() = 0;

    //returns if the object is currently being rendered
    bool is_rendering_enabled() const;

    //sets if this object is being rendered.
    void set_rendering_enabled(bool r);

    LAYERS get_render_layer();

private:
    

protected:
    LAYERS mRender_layer;
    bool mRendering_enabled;
};
