#pragma once
#include "LTexture.h"
#include "Renderable.h"
#include "../proto/map.pb.h"

class Map : Renderable
{
public:
	//for the editor, loads an empty map
	Map();
	explicit Map(std::string map_path);
	~Map();

	int get_height() const;
	int get_width() const;

	//reads a map from a map file
	//returns true on success and false otherwise
	bool deserialize(std::string& path);

	void render() override;

	TILETYPES get_resource_at_tile(const int x, const int y) const;

	towerdefense::map::Monsterpath get_monster_path(int index) const;
	
protected:
	void update_map_texture() const;

	//the map data structure
	towerdefense::map mMap;

	int mOffset_left;
	int mOffset_top;
	int mWidth;
	int mHeight;
	int mLayer_count;

	//members that are deserialized from the map file
	int mTile_size_x;
	int mTile_size_y;
	
	//the finished texture, consisting of the background layer with added decorations
	LTexture *mMap_texture;
	LTexture *mBackground_texture;

private:

};
