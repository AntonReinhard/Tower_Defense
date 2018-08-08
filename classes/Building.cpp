#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "Window.h"
#include "LayerHandler.h"

Building::Building(std::string building_name, SDL_Point coords, Level* level)
{
	mName = building_name;
	mSprite_path = std::string(gConfig_file->Value(mName + "/sprite", "path"));

	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(mSprite_path);
	mSprite_dimensions.w = gConfig_file->Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = gConfig_file->Value(building_name + "/sprite", "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	//set the maintenance costs of the building
	mMaintenance.set_ressources(gConfig_file->Value(building_name + "/stats", "goldMain"),
		gConfig_file->Value(building_name + "/stats", "woodMain"),
		gConfig_file->Value(building_name + "/stats", "stoneMain"),
		gConfig_file->Value(building_name + "/stats", "ironMain"),
		gConfig_file->Value(building_name + "/stats", "energyMain"),
		gConfig_file->Value(building_name + "/stats", "waterMain"),
		gConfig_file->Value(building_name + "/stats", "foodMain"));

	mElapsed_ticks = 0;

	mLevel = level;
	mCoords = coords;

	SDL_Rect clickable;
	clickable.x = coords.x - mSprite_dimensions.w/2;
	clickable.y = coords.y - mSprite_dimensions.h/2;
	clickable.w = mSprite_dimensions.w;
	clickable.h = mSprite_dimensions.h;
	this->set_clickable_space(clickable);
	idle = false;
}

Building::~Building()
{
	//don't destroy texture, handled by texture class
}


void Building::render()
{
	SDL_Rect dest;

	dest.x = mCoords.x - mSprite_dimensions.w / 2;
	dest.y = mCoords.y - mSprite_dimensions.h / 2;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	
	gLayer_handler->render_to_layer(mSprite, LAYERS::BUILDINGS, nullptr, &dest);
}

void Building::on_tick()
{
	if(mElapsed_ticks % 60 == 0)
	{
		idle = !mLevel->get_ressources()->sub(&mMaintenance);
	}
	mElapsed_ticks++;
}


void Building::set_maintenance(Ressources new_maintenance)
{
	mMaintenance = new_maintenance;
}

void Building::on_click(int mouse_x, int mouse_y)
{
	printf("Building clicked: %s\n", this->mName.c_str());
}

void Building::on_mouse_over(int mouse_x, int mouse_y)
{
	printf("Building mouse over: %s\n", this->mName.c_str());
	SDL_Rect rect;
	rect.x = mouse_x;
	rect.y = mouse_y;
	rect.w = 200;
	rect.h = 200;

	//TODO: make this faster so we can uncomment it
	auto w = new Window(rect);
	w->render();
	delete w; 
}

void Building::on_right_click(int mouse_x, int mouse_y)
{
	printf("Building right click: %s\n", this->mName.c_str());
}

void Building::on_middle_click(int mouse_x, int mouse_y)
{
	printf("Building middle click: %s\n", this->mName.c_str());
}

void Building::set_coords(SDL_Point coords)
{
	mCoords = coords;
}

SDL_Point Building::get_coords() const
{
	return mCoords;
}

Ressources Building::get_maintenance() const
{
	return mMaintenance;
}


