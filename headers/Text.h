#pragma once
#include "LTexture.h"
#include "Renderable.h"
#include "Vector.h"
#include "Constants.h"

class Text 
    : public Renderable
{
public:
    Text(const std::string& text, SDL_Rect dim, LAYERS layer, SDL_Color text_color, bool rendering_enabled);
    ~Text();
    
    void set_text(const std::string& text);

    void set_opacity(double opacity);

    void render() override;

    LTexture* get_texture() const;

    void set_position(int x, int y);
    const SDL_Rect& get_dimensions() const;
    
    static std::string remove_trailing_zeros(std::string s);

    void set_updated(bool updated = true);
    bool was_updated();

private:
    LTexture* mText_texture;

    std::string mText;

    SDL_Color mText_color;

    SDL_Rect mDim;

    //flag indicating the content of the text has changed
    bool mUpdated;
};

