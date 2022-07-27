#include "LTexture.h"
#include "SDL_setup.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

LTexture::LTexture()
{
    //Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
    mPixels = nullptr;
    mPitch = 0;
    mPath = "";
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::load_from_file(const std::string& path)
{
    mPath = path;

    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* new_texture = nullptr;

    //Load image at specified path
    const auto loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        /*
        //Color key image
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
        */

        //Convert surface to display format
        auto formatted_surface = SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0);

        if (formatted_surface == nullptr)
        {
            printf("Unable to convert loaded surface to display format! %s\n", SDL_GetError());
        }
        else
        {
            //Create blank streamable texture
            new_texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formatted_surface->w, formatted_surface->h);
            if (new_texture == nullptr)
            {
                printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                //Enable blending on texture
                SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);

                //Lock texture for manipulation
                SDL_LockTexture(new_texture, &formatted_surface->clip_rect, &mPixels, &mPitch);

                //Copy loaded/formatted surface pixels
                memcpy(mPixels, formatted_surface->pixels, formatted_surface->pitch * formatted_surface->h);

                //Get image dimensions
                mWidth = formatted_surface->w;
                mHeight = formatted_surface->h;

                //Get pixel data in editable format
                const auto pixels = reinterpret_cast<Uint32*>(mPixels);
                const auto pixel_count = (mPitch / 4) * mHeight;

                //Map colors				
                const auto color_key = SDL_MapRGB(formatted_surface->format, 0, 0xFF, 0xFF);
                const auto transparent = SDL_MapRGBA(formatted_surface->format, 0x00, 0xFF, 0xFF, 0x00);

                //Color key pixels
                for (auto i = 0; i < pixel_count; ++i)
                {
                    if (pixels[i] == color_key)
                    {
                        pixels[i] = transparent;
                    }
                }

                //Unlock texture to update
                SDL_UnlockTexture(new_texture);
                mPixels = nullptr;
            }

            //Get rid of old formatted surface
            SDL_FreeSurface(formatted_surface);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }

    //Return success
    mTexture = new_texture;
    return mTexture != nullptr;
}

#ifdef _SDL_TTF_H
bool LTexture::load_from_rendered_text(const std::string& texture_text, const SDL_Color text_color)
{
    //Get rid of preexisting texture
    free();
    //Render text surface
    if (gFont == nullptr)
    {
        throw std::exception("Couldn't load font!");
    }
    const auto text_surface = TTF_RenderText_Solid(gFont, texture_text.c_str(), text_color);
    if (text_surface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
        if (mTexture == nullptr)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = text_surface->w;
            mHeight = text_surface->h;

            SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
        }

        //Get rid of old surface
        SDL_FreeSurface(text_surface);
    }

    //Return success
    return mTexture != nullptr;
}
#endif

bool LTexture::create_blank(const int width, const int height, const SDL_TextureAccess access)
{
    //Create uninitialized texture
    mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if(mTexture == nullptr)
    {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }
    return mTexture != nullptr;
}

bool LTexture::load_from_surface(SDL_Surface* surface)
{
    //get rid of old texture
    free();

    this->mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);

    this->mWidth = surface->w;
    this->mHeight = surface->h;

    return mTexture != nullptr;
}

void LTexture::free()
{
    //Free texture if it exists
    if (mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::set_color(const Uint8 red, const Uint8 green, const Uint8 blue) const
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::set_blend_mode(const SDL_BlendMode blending) const
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::set_alpha(const Uint8 alpha) const
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Rect* dest, SDL_Rect* clip, const double angle, SDL_Point* center, const SDL_RendererFlip flip) const
{
    //Set rendering space and render to screen
    SDL_Rect render_quad;
    if (dest->w == 0 || dest->h == 0)
        render_quad = { dest->x, dest->y, mWidth, mHeight };
    else
        render_quad = { dest->x, dest->y, dest->w, dest->h };

    //Set clip rendering dimensions
    if (clip != nullptr)
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &render_quad, angle, center, flip);
}

void LTexture::set_as_render_target() const
{
    //Make self render target
    SDL_SetRenderTarget(gRenderer, mTexture);
}

int LTexture::get_width() const
{
    return mWidth;
}

int LTexture::get_height() const
{
    return mHeight;
}

bool LTexture::lock_texture()
{
    auto success = true;

    //Texture is already locked
    if (mPixels != nullptr)
    {
        printf("Texture is already locked!\n");
        success = false;
    }
    //Lock texture
    else
    {
        if (SDL_LockTexture(mTexture, nullptr, &mPixels, &mPitch) != 0)
        {
            printf("Unable to lock texture! %s\n", SDL_GetError());
            success = false;
        }
    }

    return success;
}

bool LTexture::unlock_texture()
{
    auto success = true;

    //Texture is not locked
    if (mPixels == nullptr)
    {
        printf("Texture is not locked!\n");
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture(mTexture);
        mPixels = nullptr;
        mPitch = 0;
    }

    return success;
}

void* LTexture::get_pixels() const
{
    return mPixels;
}

void LTexture::copy_pixels(void* pixels) const
{
    //Texture is locked
    if (mPixels != nullptr)
    {
        //Copy to locked pixels
        memcpy(mPixels, pixels, mPitch * mHeight);
    }
}

int LTexture::get_pitch() const
{
    return mPitch;
}

Uint32 LTexture::get_pixel32(const unsigned int x, const unsigned int y) const
{
    //Convert the pixels to 32 bit
    const auto pixels = reinterpret_cast<Uint32*>(mPixels);

    //Get the pixel requested
    return pixels[(y * (mPitch / 4)) + x];
}

std::string& LTexture::get_path()
{
    return mPath;
}
