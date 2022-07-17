#pragma once
#include "Defense.h"
#include "LTexture.h"
#include "Entity.h"
#include "Vector.h"
#include "Enums.h"
#include "Clickable.h"
#include "Window.h"

#include <SDL.h>
#include <vector>
#include <memory>
#include <array>

class Level;

/*
 * A Unit
 * Can walk over the screen with animation
 * Has defensive stats and movement speed
 */
class Unit : public Entity, public Clickable
{
public:
    Unit(const std::string& unit_name, Level* level, LAYERS render_layer);
    ~Unit();

    void render() override;

    Vector get_hit_box_offset() const;
    int get_hitbox_radius() const;
    Defense* get_defense() const;
    double get_move_speed() const;

protected:
    void update_animation_clip();
    double get_rotation_angle() const;

    std::shared_ptr<Window> create_window();

    void on_click(int mouse_x, int mouse_y) override;

    std::string mName;
    
    Defense* mDefense;
    double mMove_speed;
    
    Vector mPosition;
    Vector mHitbox_offset;
    int mHitbox_radius;
    //Only up, right, down or left for the choosing of the right sprite
    DIRECTION mDirection;

    SDL_Point mCenter;
    int mAnimation_tick;
    int mAnimation_tick_count;
    int mTickcount_per_clip;
    LTexture *mSprite;
    std::vector<SDL_Rect> mClips;
    SDL_Rect mCurrent_clip;
    SDL_Rect mSprite_dimensions;
    Level* mLevel;

    std::array<std::shared_ptr<Text>, RESISTANCES_TOTAL> mDefense_values;
    std::shared_ptr<Text> mHealth_value;
    std::shared_ptr<Text> mMove_speed_value;
};
