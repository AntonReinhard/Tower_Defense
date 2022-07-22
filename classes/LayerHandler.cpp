#include <LayerHandler.h>
#include <SDL.h>
#include "SDL_setup.h"

#include <iostream>
#include <optional>

LayerHandler* gLayer_handler = nullptr;

RenderTexture::RenderTexture(LTexture* texture, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect) 
    : texture{ texture }
    , src_rect{}
    , dst_rect{}
    , ex{false}
    , angle{ 0.0 }
    , center{}
    , flip{SDL_FLIP_NONE}
{
    //if no src rect is given, the whole texture is the src_rect
    //if src_rect.x is negative one then on render it will just use a nullptr
    this->src_rect = src_rect.value_or(SDL_Rect{ -1, 0, 0, 0 });

    //if no dst_rect is given, the whole screen is the dst_rect
    this->dst_rect = dst_rect.value_or(SDL_Rect{ 0, 0 });
}

RenderTexture::RenderTexture(LTexture* texture, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect, const double angle, const std::optional<SDL_Point> center, const SDL_RendererFlip flip)
    : texture{ texture }
    , src_rect{}
    , dst_rect{}
    , ex{ true }
    , angle{ angle }
    , center{}
    , flip{ flip }
{
    //if no src rect is given, the whole texture is the src_rect
    //if src_rect.x is negative one then on render it will just use a nullptr
    this->src_rect = src_rect.value_or(SDL_Rect{ -1, 0, 0, 0 });

    //if no dst_rect is given, the whole screen is the dst_rect
    this->dst_rect = dst_rect.value_or(SDL_Rect{ 0, 0, 0, 0 });

    this->center = center.value_or(SDL_Point{ 0, 0 });
}

LayerHandler::LayerHandler()
{
    this->init_mpairs();
}

LayerHandler::~LayerHandler() = default;

void LayerHandler::render_to_layer(LTexture* texture, const LAYERS layer, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect)
{
    //create the RenderTexture object with all the necessary meta data
    //insert the texture into the vector at the right layer
    this->mPairs.at(layer).emplace_back(RenderTexture(texture, src_rect, dst_rect));
}

void LayerHandler::renderex_to_layer(LTexture* texture, const LAYERS layer, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect, const double angle, const std::optional<SDL_Point> center, const SDL_RendererFlip flip)
{
    //create the RenderTexture object with all the necessary meta data
    //insert the texture into the vector at the right layer
    this->mPairs.at(layer).emplace_back(RenderTexture(texture, src_rect, dst_rect, angle, center, flip));
}

void LayerHandler::present()
{
    //Reset render target
    SDL_SetRenderTarget(gRenderer, nullptr);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    for (auto i = 0; i < int(LAYERS::LAYERS_TOTAL); i++)
    {
        auto& x = mPairs.at(LAYERS(i));

        for (auto r : x)
        {
            SDL_Rect* src = nullptr;
            if (r.src_rect.x != -1)
            {
                src = new SDL_Rect(r.src_rect);
            }

            r.texture->render(&r.dst_rect, src, r.angle, &r.center, r.flip);
        }
        x.clear();
    }
    SDL_RenderPresent(gRenderer);
}

void LayerHandler::init_mpairs()
{
    for (auto i = 0; i < int(LAYERS::LAYERS_TOTAL); i++)
    {
        this->mPairs.insert(std::pair<LAYERS, std::vector<RenderTexture>>(LAYERS(i), std::vector<RenderTexture>()));
    }
}
