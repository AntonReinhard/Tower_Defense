#pragma once
#include <SDL.h>

#include "Clickable.h"
#include "Renderable.h"
#include "Enums.h"
#include "Text.h"
#include <vector>

/*
 * This class implements a window of given dimensions
 */
class Window : public Renderable, public Clickable
{
public:
	explicit Window(SDL_Rect dim, LAYERS click_layer, LAYERS render_layer, STYLE style = STYLE::STANDARD);
	~Window();

	void render() override;

	void create_window_texture() const;
	
	void add_text_to_window(Text* text);

	void update_text_texture();

	void set_dim(SDL_Rect dim);
	SDL_Rect get_dim() const;

private:
	//the style of the window
	STYLE mStyle;

	//pointers to all the textures used
	LTexture* mBottom_left_corner;
	LTexture* mBottom_right_corner;
	LTexture* mTop_left_corner;
	LTexture* mTop_right_corner;
	LTexture* mHorizontal_border;
	LTexture* mVertical_border;
	LTexture* mBackground;

	int mCorner_width;
	int mCorner_height;
	int mBorder_thickness;

	std::vector<Text*> mTexts; //Texts that are shown in the window

	LTexture* mWindow_texture;
	LTexture* mText_texture;

	bool mRerender;

protected:
	//the dimensions of the window
	SDL_Rect mDim;
};