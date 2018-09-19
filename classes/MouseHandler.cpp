#include <MouseHandler.h>
#include <SDL.h>
#include <vector>
#include "LTexture.h"
#include "LayerHandler.h"

MouseHandler* gMouse_handler = nullptr;

MouseHandler::MouseHandler() : mClickables{}
{
	mItem_on_mouse = nullptr;
}

MouseHandler::~MouseHandler()
{
	mClickables.clear();
}

void MouseHandler::add_clickable(Clickable* c)
{
	this->mClickables.push_back(c);
}

void MouseHandler::del_clickable(Clickable* c)
{
	for (auto it = mClickables.begin(); it != mClickables.end(); )
	{
		if (*it == c)
		{
			it = mClickables.erase(it);
			break;
		}
	}
}

//handles the mouse_over events, since they are not (always) an SDL_Event
void MouseHandler::update()
{
	int mouse_x, mouse_y;
	//get coordinates
	SDL_GetMouseState(&mouse_x, &mouse_y);

	auto end = this->mClickables.end();

	for (auto it = this->mClickables.begin(); it != end; ++it)
	{
		auto rect = (*it)->get_clickable_space();
		if (mouse_x > rect.x && mouse_x < rect.x + rect.w && 
			mouse_y > rect.y && mouse_y < rect.y + rect.h)
		{
			(*it)->on_mouse_over(mouse_x, mouse_y);
			(*it)->set_state(LClickableState::MOUSE_OVER);
		} 
		else 
		{
			(*it)->set_state(LClickableState::MOUSE_OUT);
		}
	}
}

void MouseHandler::handle_event(SDL_Event *e)
{
	auto end = this->mClickables.end();
	auto x = e->button.x;
	auto y = e->button.y;
	for(auto i = 0; i<mClickables.size(); i++)
	{
		Clickable* it = mClickables.at(i);
		auto rect = it->get_clickable_space();
		if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (e->button.button == 1) {
					it->set_state(LClickableState::MOUSE_DOWN_LEFT);
					it->on_click(x, y);
				}
				else if (e->button.button == 2)
				{
					it->set_state(LClickableState::MOUSE_DOWN_MIDDLE);
					it->on_middle_click(x, y);
				}
				else if (e->button.button == 3)
				{
					it->set_state(LClickableState::MOUSE_DOWN_RIGHT);
					it->on_right_click(x, y);
				}
				break;
			case SDL_MOUSEMOTION:
				it->set_state(LClickableState::MOUSE_OVER);
				it->on_mouse_over(x, y);
				break;
			default:
				//printf("Unhandled Event!\n");
				break;
			}
		}
		else 
		{
			it->set_state(LClickableState::MOUSE_OUT);
		}
	}
}

void MouseHandler::render(SDL_Rect dest, SDL_Point mouse_position, LTexture* sprite)
{
	dest.x = mouse_position.x - sprite->get_width() / 2;
	dest.y = mouse_position.y - sprite->get_height() / 2;
	gLayer_handler->render_to_layer(sprite, LAYERS::OVERLAY, nullptr, &dest);
}
<<<<<<< HEAD

Clickable* MouseHandler::get_item_on_mouse()
{
	return this->mItem_on_mouse;
}

void MouseHandler::set_item_on_mouse(Clickable* item)
{
	if(item != nullptr)
	{
		item->set_state(MOUSE_OUT);
	}
	this->mItem_on_mouse = item;
}



=======
>>>>>>> f6e2745cf2f6de1479f8fb1bbecd57b07ec75a50
