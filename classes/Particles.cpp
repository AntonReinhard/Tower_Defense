#include "Particles.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"

Particle::Particle(const std::string& particle_name, const Vector& pos, const Vector& dir, const float rot, const float rot_speed)
    : Entity(PARTICLES), mCurrent_clip(), mCurrent_rotation{ rot }, mRotation_speed{ rot_speed }, mPosition { pos }, mSpeed_and_direction{ dir }
{
    const auto section = "animation/" + particle_name;

    mTexture = gTextures->get_texture(gConfig_file->value(section, "path"));
    const std::string flip_text = gConfig_file->value(section, "flip");
    
    if (flip_text == "none") mFlip = SDL_FLIP_NONE;
    else if (flip_text == "vertical") mFlip = SDL_FLIP_VERTICAL;
    else if (flip_text == "horizontal") mFlip = SDL_FLIP_HORIZONTAL;
    else mFlip = SDL_FLIP_NONE;

    //initialize the clips
    const int clip_width = gConfig_file->value(section, "clip_width");
    const int clip_height = gConfig_file->value(section, "clip_height");
    const int frames_total = gConfig_file->value(section, "frames_total");
    const auto frames_vertical = mTexture->get_height() / clip_height;

    for (auto i = 0; i < mTexture->get_height(); i += clip_height)
    {
        for (auto j = 0; j < mTexture->get_width(); j += clip_width)
        {
            SDL_Rect clip;
            clip.x = j;
            clip.y = i;
            clip.w = clip_width;
            clip.h = clip_height;
            mClips.push_back(clip);
            // break if 
            if (frames_total - 1 >= i*frames_vertical + j) {
                break;
            }
        }
    }
    mTick = 0;
    mTickcount_per_clip = gConfig_file->value(section, "tickcount_per_clip");
    mLife_ticks = mTickcount_per_clip * mClips.size();
}

Particle::~Particle()
{
    mClips.clear();
}

void Particle::render()
{
    update_animation();

    SDL_Rect dest;

    dest.x = static_cast<int>(mPosition.x - mCurrent_clip.w / 2.);
    dest.y = static_cast<int>(mPosition.y - mCurrent_clip.h / 2.);
    dest.w = mCurrent_clip.w;
    dest.h = mCurrent_clip.h;

    SDL_Point center;
    center.x = static_cast<int>(mCurrent_clip.w / 2.);
    center.y = static_cast<int>(mCurrent_clip.h / 2.);
    gLayer_handler->renderex_to_layer(this->mTexture, mRender_layer, this->mCurrent_clip, dest, this->mCurrent_rotation, center, this->mFlip);
}

void Particle::on_tick()
{
    //if lifetime is over -> suicide
    if (mTick >= mLife_ticks - 1)
    {
        delete this;
    }
}

void Particle::update_animation()
{
    //rotate
    this->mCurrent_rotation += this->mRotation_speed;
    if (this->mCurrent_rotation >= 360.0) this->mCurrent_rotation -= 360;
    if (this->mCurrent_rotation < 0.0) this->mCurrent_rotation += 360;

    //move
    this->mPosition = this->mSpeed_and_direction + this->mPosition;

    this->mTick++;
    if (this->mTick >= this->mLife_ticks)
    {
        this->mTick--;
    }
    const auto index = this->mTick / this->mTickcount_per_clip;
    this->mCurrent_clip = this->mClips.at(index);
}
