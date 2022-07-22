#include "Shadow.h"
#include "Textures.h"
#include "LayerHandler.h"
#include "SDL_setup.h"
#include "ConfigFile.h"

#include <string>
#include <chrono>
#include <cmath>

static constexpr double ROTATION_LIMIT = 0.05;
static constexpr double SPEED_LIMIT = 0.3;

namespace {
    int mod(int k, int n) {
        return ((k %= n) < 0) ? k + n : k;
    }

    double random(double lower, double upper) {
        double ret = static_cast<double>(std::abs(std::rand())) / RAND_MAX;

        return ret * (upper - lower) + lower;
    }
}

Shadow::Shadow(const std::string& name) 
    : Entity(LAYERS::OVERLAY) {
	const std::string section = "shadow/" + name;
	mTexture = gTextures->get_texture(gConfig_file->value(section, "path"));

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
            if (frames_total - 1 >= i * frames_vertical + j) {
                break;
            }
        }
    }

    mCurrent_clip = mClips[0];

    mPosition.x = 0;
    mPosition.y = 0;

    mSpeed_and_direction.x = std::rand() % 100 - 50;
    mSpeed_and_direction.y = std::rand() % 100 - 50;

    mSpeed_and_direction = (mSpeed_and_direction / mSpeed_and_direction.abs()) * SPEED_LIMIT;
}

Shadow::~Shadow() {
	gTextures->del_texture(mTexture->get_path());
}

void Shadow::render()
{
    update_animation();

    SDL_Point center;
    center.x = std::lround(mCurrent_clip.w / 2.);
    center.y = std::lround(mCurrent_clip.h / 2.);

    SDL_Rect dest;
    dest.w = mCurrent_clip.w;
    dest.h = mCurrent_clip.h;


    dest.x = mod(std::lround(mPosition.x), mCurrent_clip.w);
    if (dest.x > 0) dest.x -= mCurrent_clip.w;

    while (dest.x <= LOGICAL_SCREEN_WIDTH) {
        dest.y = mod(std::lround(mPosition.y), mCurrent_clip.h);
        if (dest.y > 0) dest.y -= mCurrent_clip.h;

        while (dest.y <= LOGICAL_SCREEN_HEIGHT) {
            gLayer_handler->renderex_to_layer(mTexture, mRender_layer, mCurrent_clip, dest, 0, center, SDL_FLIP_NONE);

            dest.y += mCurrent_clip.h;
        }

        dest.x += mCurrent_clip.w;
    }
}

void Shadow::on_tick()
{
}

void Shadow::update_animation()
{
    static std::chrono::system_clock::time_point last_speed_update(std::chrono::seconds(0));
    static double rot_vel = 0.;
    static double rot_acc = 0.;

    if (std::chrono::system_clock::now() - last_speed_update > std::chrono::seconds(3)) {
        rot_acc = random(-rot_vel, -rot_vel + ROTATION_LIMIT / 2.);

        last_speed_update = std::chrono::system_clock::now();
    }

    const double x = mSpeed_and_direction.x;
    const double y = mSpeed_and_direction.y;

    rot_vel += rot_acc / 1000.;

    mSpeed_and_direction.x = std::cos(rot_vel / 60.) * x - std::sin(rot_vel / 60.) * y;
    mSpeed_and_direction.y = std::sin(rot_vel / 60.) * x + std::cos(rot_vel / 60.) * y;

    //move
    mPosition = mPosition + mSpeed_and_direction;

    //keep position in the screen
    if (mPosition.x < 0) mPosition.x += LOGICAL_SCREEN_WIDTH;
    if (mPosition.y < 0) mPosition.y += LOGICAL_SCREEN_HEIGHT;

    // allow animations?
}
