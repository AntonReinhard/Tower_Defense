#pragma once
#include "Renderable.h"

#include <vector>

class RenderableHandler
{
public:
    RenderableHandler();
    ~RenderableHandler();

    void add_renderable(Renderable* r);
    void del_renderable(Renderable* r);

    void render();

private:
    std::vector<Renderable*> mRenderables;
};

extern RenderableHandler* gRenderables_handler;