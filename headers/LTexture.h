#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool load_from_file(const std::string& path);

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
    //Creates image from font string
    bool load_from_rendered_text(const std::string& texture_text, SDL_Color text_color);
#endif
    
    //Creates blank texture 
    bool create_blank( int width, int height, SDL_TextureAccess = SDL_TEXTUREACCESS_STREAMING );
    
    //Creates image from SDL_Surface. The surface is not gonna be destroyed at the end
    bool load_from_surface(SDL_Surface* surface);

    //Deallocates texture
    void free();

    //Set color modulation
    void set_color(Uint8 red, Uint8 green, Uint8 blue) const;

    //Set blending
    void set_blend_mode(SDL_BlendMode blending) const;

    //Set alpha modulation
    void set_alpha(Uint8 alpha) const;

    //Renders texture at given point
    void render(SDL_Rect* dest, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    
    //Set self as render target 
    void set_as_render_target() const;
    
    //Gets image dimensions
    int get_width() const;
    int get_height() const;

    //Pixel manipulators 
    bool lock_texture(); 
    bool unlock_texture(); 
    void* get_pixels() const; 
    void copy_pixels( void* pixels ) const; 
    int get_pitch() const; 
    Uint32 get_pixel32( unsigned int x, unsigned int y ) const;

    std::string& get_path();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    void* mPixels;
    int mPitch;

    //Image dimensions
    int mWidth;
    int mHeight;

    std::string mPath;
};
