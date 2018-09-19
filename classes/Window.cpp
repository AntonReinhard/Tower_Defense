#include "Window.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"


Window::Window(SDL_Rect dim, STYLE style)
{
	mDim = dim;
	mStyle = style;

	auto category = "frames/" + std::to_string(int(style));
	mBlcorner = gTextures->get_texture(gConfig_file->Value(category, "blcpath"));
	mBrcorner = gTextures->get_texture(gConfig_file->Value(category, "brcpath"));
	mTlcorner = gTextures->get_texture(gConfig_file->Value(category, "tlcpath"));
	mTrcorner = gTextures->get_texture(gConfig_file->Value(category, "trcpath"));
	mHorizontalborder = gTextures->get_texture(gConfig_file->Value(category, "hbpath"));
	mVerticalborder = gTextures->get_texture(gConfig_file->Value(category, "vbpath"));
	mBackground = gTextures->get_texture(gConfig_file->Value(category, "bgcolorpath"));

	mCorner_height = gConfig_file->Value(category, "corner_height");
	mCorner_width = gConfig_file->Value(category, "corner_width");
	mBorder_thickness = gConfig_file->Value(category, "border_thickness");
}

Window::~Window()
{
	//Don't destroy textures here
}

void Window::render() const
{
	//draw the inner color of the window (background of the window)
	SDL_Rect dest;
	dest.w = mDim.w;
	dest.h = mDim.h;

	dest.x = mDim.x;
	dest.y = mDim.y;

	gLayer_handler->render_to_layer(mBackground, LAYERS::OVERLAY, nullptr, &dest);

	//draw the four corners
	gLayer_handler->render_to_layer(mTlcorner, LAYERS::OVERLAY, nullptr, &dest);
	dest.x = mDim.x + mDim.w - mCorner_width;
	gLayer_handler->render_to_layer(mTrcorner, LAYERS::OVERLAY, nullptr, &dest);
	dest.y = mDim.y + mDim.h - mCorner_height;
	gLayer_handler->render_to_layer(mBrcorner, LAYERS::OVERLAY, nullptr, &dest);
	dest.x = mDim.x;
	gLayer_handler->render_to_layer(mBlcorner, LAYERS::OVERLAY, nullptr, &dest);

	//draw the borders
	//horizontal top:
	dest.y = mDim.y;
	dest.w = 1;
	dest.h = mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		gLayer_handler->render_to_layer(mHorizontalborder, LAYERS::OVERLAY, nullptr, &dest);
	}
	//horizontal bottom:
	dest.y = mDim.y + mDim.h - mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		gLayer_handler->render_to_layer(mHorizontalborder, LAYERS::OVERLAY, nullptr, &dest);
	}
	//vertical left:
	dest.x = mDim.x;
	dest.w = mBorder_thickness;
	dest.h = 1;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		gLayer_handler->render_to_layer(mVerticalborder, LAYERS::OVERLAY, nullptr, &dest);
	}
	//vertical right
	dest.x = mDim.x + mDim.w - mBorder_thickness;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		gLayer_handler->render_to_layer(mVerticalborder, LAYERS::OVERLAY, nullptr, &dest);
	}
	SDL_Color text_color = { 0, 255, 255 };

	LTexture* text = new LTexture();
	text->load_from_rendered_text("b,iae,amiuembiumbei", text_color);
	


	gLayer_handler->render_to_layer(text, LAYERS::OVERLAY, nullptr, &dest);

}

void Window::set_dim(SDL_Rect dim)
{
	mDim = dim;
}