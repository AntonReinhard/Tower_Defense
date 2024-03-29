#pragma once

#include <SDL.h>

#include <string>

static constexpr double EPSILON = 0.0001;

static constexpr SDL_Color COLOR_NEUTRAL = { 0, 0, 0 };
static constexpr SDL_Color COLOR_NEGATIVE = { 232, 13, 13 };
static constexpr SDL_Color COLOR_POSITIVE = { 49, 238, 0 };

enum BUILDINGWINDOWBUTTONIDS {
    DEMOLISH_BUTTON,
    UPGRADE_BUTTON,
    UPGRADE_DAMAGE_BUTTON,
    UPGRADE_RANGE_BUTTON,
    UPGRADE_ATTACKSPEED_BUTTON,
    SHOW_MORE_BUTTON,
    BUTTON_NUMBER
};

enum BUILDINGTYPE {
    TOWER,
    INDUSTRIAL_BUILDING,
    WAREHOUSE,
    STREET,
    BUILDINGTYPES_TOTAL
};

enum TILETYPES {
    EMPTY,
    MONSTERPATH,
    WOODTILE,
    IRONTILE,
    TILETYPES_TOTAL
};

enum ACTIVITY {
    GETTING_IDLE,
    GETTING,
    DELIVERING_IDLE,
    DELIVERING,
    ACTIVITIES_TOTAL,e
};

enum BUILDINGDIRECTION {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    BUILDINGDIRECTIONS_TOTAL
};

enum RESOURCETYPES {
    GOLD,
    WOOD,
    STONE,
    IRON,
    ENERGY,
    WATER,
    FOOD,
    RESOURCES_TOTAL
};

enum RESISTANCES
{
    ARMOR,
    MAGIC_RES,
    FIRE_RES,
    WATER_RES,
    ELEC_RES,
    RESISTANCES_TOTAL
};

enum PRODUCTIONSTATE {
    NONE,
    PRODUCING,
    CONSUMING
};

//The numbers are the row positions of the rotations in the sprite image (outdated)
enum DIRECTION {
    DOWN = 4,
    LEFT = 5,
    UP = 6,
    RIGHT = 7
}; 

enum L_CLICKABLE_STATE {
    MOUSE_OUT = 0,			//while the mouse is not over the clickable
    MOUSE_OVER = 1,			//while the mouse hovers over the clickable
    MOUSE_DOWN_LEFT = 2,	//while left mouse button is pressed
    MOUSE_DOWN_RIGHT = 3,	//while right mouse button is pressed
    MOUSE_DOWN_MIDDLE = 4,	//while middle mouse button is pressed
    MOUSE_UP = 5,			//one tick after a mouse click
    STATES_TOTAL = 6
};

enum ENTITYTYPE {
    UNDEFINED,
    BUILDING,
    ENEMY,
    SHOT,
    PARTICLE,
    ENTITYTYPES_TOTAL
};

enum LAYERS {
    BACKGROUND,
    BUILDINGS,
    BUILDINGOVERLAY,
    ENEMIES,
    SHOTS,
    PARTICLES,
    OVERLAY,
    TEXTPARTICLES,
    WINDOWS,
    WINDOWCONTENT,
    UPGRADEWINDOW,
    UPGRADEWINDOWCONTENT,
    LAYERS_TOTAL
};

enum STYLE {
    STANDARD,
    STYLES_TOTAL
};

enum LEVEL_SELECT_BUTTONS
{
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LBACK,
    TOTAL
};