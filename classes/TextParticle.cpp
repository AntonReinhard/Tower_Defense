#include "TextParticle.h"
#include "SDL_setup.h"

#include <chrono>

static constexpr std::chrono::duration START_TRANSPARENCY = std::chrono::seconds(1);

namespace {
	double random(double lower, double upper) {
		double ret = static_cast<double>(std::abs(std::rand())) / RAND_MAX;

		return ret * (upper - lower) + lower;
	}
}

TextParticle::TextParticle(const std::string& text, const SDL_Color& color, const Vector& pos)
	: Entity(TEXTPARTICLES)
	, mPosition(pos)
	, mOpacity(1)
	, mCreation(std::chrono::system_clock::now()){
	SDL_Rect text_dest = { pos.x, pos.y, 0, 0 };
	mText = std::make_unique<Text>(text, text_dest, get_render_layer(), color, false);

	// center position of the text
	mText->set_position(pos.x - mText->get_texture()->get_width() / 2, pos.y - mText->get_texture()->get_height());

	// float to the side and up (pixels per second)
	mSpeed.x = random(-20, 20);
	mSpeed.y = random(-10, 1);
}

TextParticle::~TextParticle() = default;

void TextParticle::render() {

	if (std::chrono::system_clock::now() - mCreation < START_TRANSPARENCY) {
		// accelerate up
		mSpeed.y -= 5. / *gFrame_rate;
	}
	else {
		mOpacity -= 0.5 / *gFrame_rate;
	}

	mPosition = mPosition + mSpeed / *gFrame_rate;
	
	mText->set_position(mPosition.x, mPosition.y);
	mText->set_opacity(mOpacity);

	if (mOpacity >= 0.)	mText->render();
}

void TextParticle::on_tick() {
	if (mOpacity <= 0) {
		delete this;
	}
}
