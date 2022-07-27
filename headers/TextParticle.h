#include "Entity.h"
#include "Text.h"

#include <SDL.h>

#include <chrono>
#include <memory>

class TextParticle : Entity
{
public:

	TextParticle(const std::string& text, const SDL_Color& color, const Vector& pos);

	~TextParticle();

	void render() override;
	void on_tick() override;

protected:

private:
	void update_animation();

	std::unique_ptr<Text> mText;

	Vector mPosition;

	Vector mSpeed;

	double mOpacity;

	const std::chrono::system_clock::time_point mCreation;
	
};
