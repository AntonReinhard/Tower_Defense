#include "Textures.h"
#include "SDL_setup.h"
#include "LTexture.h"

Textures::~Textures()
{
    for (auto& it : mContent)
    {
        //Getting rid of all the left textures
        it.second->free();
    }
    //empty the map itself
    mContent.clear();
}

LTexture* Textures::get_texture(std::string path)
{
    //search for the path as key
    const auto it = mContent.find(path);
    if (it != mContent.end())	//if the name is found -> it's already loaded
    {
        return it->second;		//return the stored pointer to the LTexture
    }

    //if not yet loaded -> load now

    auto tex = new LTexture();
    if (!tex->load_from_file(path))
    {
        printf("Couldn't load texture at %s!\n", path.c_str());
    }
    
    mContent.insert(std::pair<std::string, LTexture*>(path, tex));	//insert key and value in the map

    return tex;
}

bool Textures::del_texture(const std::string& path)
{
    auto it = mContent.find(path);
    if (it != mContent.end())
    {
        //deallocate memory
        it->second->free();
        //destroy the entry in the map
        mContent.erase(it);
        return true;
    }

    // if it's not yet in the map, it can't be deleted
    return false;
}
