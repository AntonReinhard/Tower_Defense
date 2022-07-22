#pragma once
#include <vector>
#include <SDL.h>
#include "LTexture.h"
#include "Enums.h"

#include <map>
#include <optional>

//this class holds all the attributes needed to render a texture right
struct RenderTexture
{
    RenderTexture(LTexture* texture, std::optional<SDL_Rect> src_rect, std::optional<SDL_Rect> dst_rect);
    RenderTexture(LTexture* texture, std::optional<SDL_Rect> src_rect, std::optional<SDL_Rect> dst_rect, double angle, const std::optional<SDL_Point> center, SDL_RendererFlip flip);

    LTexture* texture;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    bool ex;
    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;
};

//the LayerHandler manages textures that are to be rendered.
class LayerHandler
{
public:
    LayerHandler();
    ~LayerHandler();

    //renders everything currently in mPairs in the right order to the screen
    void present();

    //adds a texture to be shown on a specified layer in the next frame
    void render_to_layer(LTexture* texture, LAYERS layer, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect);
    void renderex_to_layer(LTexture* texture, LAYERS layer, const std::optional<SDL_Rect> src_rect, const std::optional<SDL_Rect> dst_rect, double angle, std::optional<SDL_Point> center, SDL_RendererFlip flip);

private:

    //puts empty vectors into mPairs along with every Layer
    void init_mpairs();

    //holds the textures that are going to be shown on different layers
    std::map<LAYERS, std::vector<RenderTexture>> mPairs;

};  

extern LayerHandler* gLayer_handler;
          