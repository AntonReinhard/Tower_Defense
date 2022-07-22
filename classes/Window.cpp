#include "Window.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "MouseHandler.h"

#include <memory>

Window::Window(const SDL_Rect dim, const LAYERS click_layer, const LAYERS render_layer, const STYLE style)
    : Renderable(render_layer)
    , Clickable(click_layer)
    , mStyle(style)
    , mRerender(true)
    , mDim(dim)
{
    const auto category = "frames/" + std::to_string(int(style));
    mBottom_left_corner = gTextures->get_texture(gConfig_file->value(category, "blcpath"));
    mBottom_right_corner = gTextures->get_texture(gConfig_file->value(category, "brcpath"));
    mTop_left_corner = gTextures->get_texture(gConfig_file->value(category, "tlcpath"));
    mTop_right_corner = gTextures->get_texture(gConfig_file->value(category, "trcpath"));
    mHorizontal_border = gTextures->get_texture(gConfig_file->value(category, "hbpath"));
    mVertical_border = gTextures->get_texture(gConfig_file->value(category, "vbpath"));
    mBackground = gTextures->get_texture(gConfig_file->value(category, "bgcolorpath"));

    mCorner_height = gConfig_file->value(category, "corner_height");
    mCorner_width = gConfig_file->value(category, "corner_width");
    mBorder_thickness = gConfig_file->value(category, "border_thickness");

    mWindow_texture = new LTexture();
    mText_texture = new LTexture();

    set_clickable_space(mDim);

    create_window_texture();
}

Window::~Window() {
    mWindow_texture->free();
    mText_texture->free();
}

void Window::render()
{
    gLayer_handler->render_to_layer(mWindow_texture, WINDOWS, {}, mDim);

    if (mTexts.empty()) return;

    for (auto& text : mTexts) {
        if (text->was_updated()) {
            mRerender = true;
            break;
        }
    }

    if (mRerender) update_text_texture();

    gLayer_handler->render_to_layer(mText_texture, WINDOWCONTENT, {}, mDim);
}

void Window::create_window_texture() const
{
    if (!mWindow_texture->create_blank(mDim.w, mDim.h, SDL_TEXTUREACCESS_TARGET))
    {
        printf("Failed to create target texture!\n");
        return;
    }

    //we want to render to the texture
    mWindow_texture->set_as_render_target();

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //draw the inner color of the window (background of the window)
    SDL_Rect dest{ 0, 0, mDim.w, mDim.h };

    mBackground->render(&dest);
    dest.w = 0; dest.h = 0;

    //draw the four corners
    mTop_left_corner->render(&dest);
    dest.x = mDim.w - mCorner_width;
    mTop_right_corner->render(&dest);
    dest.y = mDim.h - mCorner_height;
    mBottom_right_corner->render(&dest);
    dest.x = 0;
    mBottom_left_corner->render(&dest);

    //draw the borders
    //horizontal top:
    dest = { mCorner_width, 0, mDim.w - 2 * mCorner_width, mBorder_thickness };

    mHorizontal_border->render(&dest);

    //horizontal bottom:
    dest.y = mDim.h - mBorder_thickness;
    //width, height and x stay the same

    mHorizontal_border->render(&dest);

    //vertical left:
    dest = { 0, mCorner_height, mBorder_thickness, mDim.h - 2 * mCorner_height };

    mVertical_border->render(&dest);

    //vertical right
    dest.x = mDim.w - mBorder_thickness;
    //width, height and y stay the same

    mVertical_border->render(&dest);

    SDL_SetRenderTarget(gRenderer, nullptr);
}

void Window::set_dim(const SDL_Rect dim)
{
    mDim = dim;
}

SDL_Rect Window::get_dim() const
{
    return mDim;
}

std::shared_ptr<Text> Window::add_text_to_window(const std::string& text, SDL_Rect dim, LAYERS layer, SDL_Color text_color)
{
    auto t = std::make_shared<Text>(text, dim, layer, text_color, false);
    mTexts.emplace_back(t);
    mRerender = true;

    return t;
}

void Window::update_text_texture()
{
    if (!mText_texture->create_blank(mDim.w, mDim.h, SDL_TEXTUREACCESS_TARGET))
    {
        printf("Failed to create target texture!\n");
        return;
    }

    mText_texture->set_as_render_target();

    // set blend modes to create transparent texture to render text on
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
    mText_texture->set_blend_mode(SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderFillRect(gRenderer, nullptr);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

    SDL_Rect dest;
    for (const auto& text : mTexts)
    {
        dest = text->get_dimensions();
        text->get_texture()->render(&dest);

        // rendered it, reset flag
        text->set_updated(false);
    }

    SDL_SetRenderTarget(gRenderer, nullptr);

    mRerender = false;
}
