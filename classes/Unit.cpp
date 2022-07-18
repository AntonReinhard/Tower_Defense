#include "Unit.h"

#include <utility>
#include "Textures.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "Menu.h"
#include "Window.h"

Unit::Unit(const std::string& unit_name, Level* level, LAYERS render_layer)
    : Entity(render_layer)
    , Clickable(render_layer)
    , mName(unit_name)
    , mCenter()
    , mCurrent_clip()
    , mSprite_dimensions()
    , mLevel(level)
{
    const auto sprite_section = mName + "/sprite";
    const auto stats_section = mName + "/stats";
    mHitbox_offset.x = gConfig_file->value_or_zero(sprite_section, "hitbox_offset_x");
    mHitbox_offset.y = gConfig_file->value_or_zero(sprite_section, "hitbox_offset_y");
    mHitbox_radius = gConfig_file->value_or_zero(sprite_section, "hitbox_radius");
    //load path of the sprite
    std::string path = gConfig_file->value(sprite_section, "path");
    mSprite = gTextures->get_texture(path);	//set the pointer to the sprite

    mSprite_dimensions.w = mSprite->get_width();
    mSprite_dimensions.h = mSprite->get_height();
    //mSprite_dimensions.w = gConfig_file->value(sprite_section, "clip_width");
    //mSprite_dimensions.h = gConfig_file->value(sprite_section, "clip_height");
    mSprite_dimensions.x = 0;
    mSprite_dimensions.y = 0;
    int clip_width = gConfig_file->value(sprite_section, "clip_width");
    int clip_height = gConfig_file->value(sprite_section, "clip_height");
    for (auto i = 0; i < mSprite_dimensions.h; i += clip_height)
    {
        for (auto j = 0; j < mSprite_dimensions.w; j += clip_width)
        {
            SDL_Rect clip{ j, i, clip_width, clip_height };
            mClips.push_back(clip);
        }
    }
    mAnimation_tick = 0;
    mTickcount_per_clip = gConfig_file->value(sprite_section, "tickcount_per_clip");

    this->mCenter.x = gConfig_file->value(sprite_section, "rotation_center_x");
    this->mCenter.y = gConfig_file->value(sprite_section, "rotation_center_y");

    // animation tick count is how many ticks it takes for one cycle of the animation to run through.
    // each clip gets shown for tickcount_per_clip ticks
    mAnimation_tick_count = mSprite_dimensions.w / clip_width * mSprite_dimensions.h / clip_height * mTickcount_per_clip;

    //gets initialized in the derived classes
    mPosition.x = 0;
    mPosition.y = 0;
    mDirection = DIRECTION::DOWN;

    mMove_speed = gConfig_file->value(stats_section, "movementspeed");
    mDefense = new Defense();
    mDefense->set_health(double(int(gConfig_file->value_or_zero(stats_section, "health"))));
    mDefense->set_full_health(double(int(gConfig_file->value_or_zero(stats_section, "health"))));
    mDefense->set_resistances(double(int(gConfig_file->value_or_zero(stats_section, "armor"))),
        double(int(gConfig_file->value_or_zero(stats_section, "magicres"))),
        double(int(gConfig_file->value_or_zero(stats_section, "fireres"))),
        double(int(gConfig_file->value_or_zero(stats_section, "waterres"))),
        double(int(gConfig_file->value_or_zero(stats_section, "elecres"))));
    mDefense->set_immunities(bool(int(gConfig_file->value_or_zero(stats_section, "physimm"))),
        bool(int(gConfig_file->value_or_zero(stats_section, "magicimm"))),
        bool(int(gConfig_file->value_or_zero(stats_section, "fireimm"))),
        bool(int(gConfig_file->value_or_zero(stats_section, "waterimm"))),
        bool(int(gConfig_file->value_or_zero(stats_section, "elecimm"))));
    //setting up the blending
    mSprite->set_blend_mode(SDL_BLENDMODE_BLEND);

    update_animation_clip();

    mClickable_space.x = int(mPosition.x);
    mClickable_space.y = int(mPosition.y);
    mClickable_space.w = gConfig_file->value(sprite_section, "clip_width");
    mClickable_space.h = gConfig_file->value(sprite_section, "clip_height");
}

Unit::~Unit()
{
    delete mDefense;
}

void Unit::render()
{
    if (mHealth_value != nullptr) {
        for (auto i = 0; i < RESISTANCES_TOTAL; i++)
        {
            mDefense_values[i]->set_text(std::to_string(int(mDefense->get_resistance(RESISTANCES(i)))));
        }
        mMove_speed_value->set_text(std::to_string(int(mMove_speed)));
        mHealth_value->set_text(std::to_string(int(mDefense->get_health())) + "/" + std::to_string(int(mDefense->get_full_health())));
    }

    mClickable_space.x = int(mPosition.x) - mCenter.x;
    mClickable_space.y = int(mPosition.y) - mCenter.y;

    update_animation_clip();

    SDL_Rect dest{ int(mPosition.x - mCenter.x), int(mPosition.y - mCenter.y),  mCurrent_clip.w, mCurrent_clip.h };
    gLayer_handler->renderex_to_layer(this->mSprite, mRender_layer, this->mCurrent_clip, dest, this->get_rotation_angle(), this->mCenter, SDL_FLIP_NONE);
}

double Unit::get_rotation_angle() const
{
    switch (this->mDirection) {
    case DIRECTION::RIGHT:
        return 0.0;
    case DIRECTION::DOWN:
        return 90.0;
    case DIRECTION::LEFT:
        return 180.0;
    case DIRECTION::UP:
        return 270.0;
    default:
        return 0.0;
    }
}

std::shared_ptr<Window> Unit::create_window()
{
    SDL_Rect rect{ 1530, 804, 200, 220 };

    auto window = std::make_shared<Window>(rect, WINDOWS, WINDOWS);

    const SDL_Color text_color{ 0,0,0,0 };

    SDL_Rect dest{ 20, 20, 0, 0 };

    dest.x += 5;
    window->add_text_to_window(mName, dest, WINDOWCONTENT, text_color);
    dest.y += 40;
    window->add_text_to_window("Health", dest, WINDOWCONTENT, text_color);
    dest.x += 95;
    mHealth_value = window->add_text_to_window(std::to_string(int(mDefense->get_health())) + "/" + std::to_string(int(mDefense->get_full_health())), dest, WINDOWCONTENT, text_color);
    dest.x -= 100;
    for (auto i = 0; i < mDefense_values.size(); i++)
    {
        dest.y += 20;
        window->add_text_to_window(Defense::get_name(RESISTANCES(i)), dest, WINDOWCONTENT, text_color);
        dest.x += 100;
        mDefense_values[i] = window->add_text_to_window(std::to_string(int(mDefense->get_resistance(RESISTANCES(i)))), dest, WINDOWCONTENT, text_color);
        dest.x -= 100;
    }
    dest.y += 20;
    window->add_text_to_window("Move Speed", dest, WINDOWCONTENT, text_color);
    dest.x += 100;
    mMove_speed_value = window->add_text_to_window(std::to_string(int(mMove_speed)), dest, WINDOWCONTENT, text_color);

    return window;
}

void Unit::update_animation_clip()
{
    mAnimation_tick++;
    if (mAnimation_tick >= mAnimation_tick_count)
    {
        mAnimation_tick = 0;
    }
    const auto index = mAnimation_tick / mTickcount_per_clip;

    mCurrent_clip = mClips.at(index);
}

void Unit::on_click(int mouse_x, int mouse_y)
{
    mLevel->get_menu()->set_unit_window(create_window());
}

Defense* Unit::get_defense() const
{
    return mDefense;
}

double Unit::get_move_speed() const
{
    return mMove_speed;
}

Vector Unit::get_hit_box_offset() const
{
    return mHitbox_offset;
}

int Unit::get_hitbox_radius() const
{
    return mHitbox_radius;
}
