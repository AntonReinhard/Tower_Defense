#include "Map.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "SDL_setup.h"
#include <SDL.h>
#include <google/protobuf/util/json_util.h>

#include <iostream>
#include <fstream>
#include <sstream>

Map::Map() : Renderable(BACKGROUND)
{
	mTile_size_x = 64;
	mTile_size_y = 64;

	mOffset_left = gConfig_file->value("map", "offset_left");
	mOffset_top = gConfig_file->value("map", "offset_top");
	mWidth = gConfig_file->value("map", "width");
	mHeight = gConfig_file->value("map", "height");
	mLayer_count = gConfig_file->value("map", "layer_count");

	mBackground_texture = gTextures->get_texture("resources/background.bmp");

	mMap_texture = new LTexture();
}


Map::Map(std::string map_path) 
	: Map()
{
	this->deserialize(map_path);
}

Map::~Map()
{
	mMap_texture->free();
	delete mMap_texture;

	for (auto& pair : mMap.tile_map())
	{
		gTextures->del_texture(pair.second);
	}
}

int Map::get_height() const
{
	return mHeight;
}

int Map::get_width() const
{
	return mWidth;
}

bool Map::deserialize(std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Couldn't read map from " << path << "." << std::endl;
		return false;
	}

	std::stringstream json_string;

	json_string << file.rdbuf();

	auto status = google::protobuf::util::JsonStringToMessage(json_string.str(), &mMap);

	update_map_texture();
	
	return status.ok();
}

void Map::render()
{
	gLayer_handler->render_to_layer(mBackground_texture, this->get_render_layer(), nullptr, nullptr);
	
	gLayer_handler->render_to_layer(mMap_texture, this->get_render_layer(), nullptr, nullptr);
}

TILETYPES Map::get_resource_at_tile(const int x, const int y) const
{
	return TILETYPES(mMap.resources().tiles(y * mMap.width() * 2 + x));
}

towerdefense::map::Monsterpath Map::get_monster_path(const int index) const
{
	return mMap.paths().at(index);
}

void Map::update_map_texture() const
{
	if (!mMap_texture->create_blank(mWidth, mHeight, SDL_TEXTUREACCESS_TARGET))
	{
		printf("Failed to create target texture!\n");
		return;
	}

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x00);
	SDL_RenderClear(gRenderer);

	//we want to render to the texture
	mMap_texture->set_as_render_target();

	//render all the tiles on all the layers to the map texture
	for (unsigned l = 0; l < mMap.layers_size(); ++l) {
		SDL_Rect dest{ 0, 0, mTile_size_x, mTile_size_y };
		towerdefense::map::Layer layer = mMap.layers(l);

		for (unsigned y = 0; y < mMap.height(); ++y) {
			for (unsigned x = 0; x < mMap.width(); ++x)	{

				const auto index = y * mMap.width() + x;
				const auto tile = layer.tiles(index);
				if (tile)
				{
					const auto tex = gTextures->get_texture(mMap.tile_map().at(layer.tiles(index)));
					tex->render(&dest);
				}

				dest.x += dest.w;
			}

			dest.x = 0;
			dest.y += dest.h;
		}
	}
}
