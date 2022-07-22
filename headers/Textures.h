#pragma once
#include <map>
#include "LTexture.h"


//A class to handle all the textures so they don't get loaded more than once
class Textures
{
public:
    ~Textures();

    //retrieving a texture
    //will load the texture if it's not yet saved
    LTexture* get_texture(std::string path);

    //deletes a loaded texture with a given name
    bool del_texture(const std::string& path);

private:
    //stores the unit name strings and the associated sprite
    std::map<std::string, LTexture*> mContent;
};

