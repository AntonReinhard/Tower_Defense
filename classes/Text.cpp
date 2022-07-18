#include <iomanip>
#include "Text.h"
#include "LayerHandler.h"

Text::Text(const std::string& text, const SDL_Rect dim, const LAYERS layer, const SDL_Color text_color, const bool rendering_enabled)
    : Renderable(layer), mText_texture(new LTexture()), mText(text), mDim(dim)
{
    mText_color = text_color;
    mText_texture->load_from_rendered_text(text, mText_color);
    set_rendering_enabled(false);
    mDim.w = mText_texture->get_width();
    mDim.h = mText_texture->get_height();
}

Text::~Text()
{
    delete mText_texture;
}

void Text::render()
{
    gLayer_handler->render_to_layer(mText_texture, mRender_layer, {}, mDim);
}

LTexture* Text::get_texture() const
{
    return mText_texture;
}

void Text::set_position(int x, int y)
{
    mDim.x = x;
    mDim.y = y;
}

const SDL_Rect& Text::get_dimensions() const
{
    return mDim;
}

void Text::set_text(const std::string& text)
{
    if (text != mText)
    {
        mText = text;
        mText_texture->load_from_rendered_text(text, mText_color);
        mDim.w = mText_texture->get_width();
        mDim.h = mText_texture->get_height();
    }
}

std::string Text::remove_trailing_zeros(std::string s)
{
    //erase all 0 at the end of the string, if a point is then at the end erase it too
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    s.erase(s.find_last_not_of('.') + 1, std::string::npos);

    //set precision(number of digits after the point)
    //can probably be done better
    auto found_point = false;
    auto precision = 2;
    std::string result;
    for (auto i : s)
    {
        if (!precision) break;
        if (found_point) precision--;
        if (i == '.') found_point = true;
        result.push_back(i);
    }

    return result;
}
