#pragma once
#include "LTexture.h"
#include "Vector.h"
#include "Entity.h"

#include <vector>

class Shadow :
    public Entity
{
public:
    Shadow(const std::string& shadow_name);
    ~Shadow();

    void render() override;
    void on_tick() override;

private:
    void update_animation();

    LTexture* mTexture;

    //stores all the rectangles of the different clips
    std::vector<SDL_Rect> mClips;

    //the clip that is currently shown
    SDL_Rect mCurrent_clip;

    //current position of the particle (may change in its lifetime)
    Vector mPosition;

    //speed and direction of the particle
    Vector mSpeed_and_direction;
};
