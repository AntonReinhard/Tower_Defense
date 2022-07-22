#pragma once
#include <vector>
#include "Renderable.h"

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