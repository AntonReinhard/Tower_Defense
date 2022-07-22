#include "RenderableHandler.h"
#include "Renderable.h"

#include <algorithm>

RenderableHandler* gRenderables_handler = nullptr;

RenderableHandler::RenderableHandler() = default;

RenderableHandler::~RenderableHandler()
{
    mRenderables.clear();
}

void RenderableHandler::add_renderable(Renderable* r)
{
    this->mRenderables.push_back(r);
}

void RenderableHandler::del_renderable(Renderable* r)
{
    mRenderables.erase(std::remove(mRenderables.begin(), mRenderables.end(), r), mRenderables.end());
}

void RenderableHandler::render()
{
    for (auto const& it: mRenderables)
    {
        if (it->is_rendering_enabled()) {
            it->render();
        }
    }
}
