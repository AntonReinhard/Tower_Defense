#include <SDL.h>
#include <vector>

#include "MouseHandler.h"
#include "SDL_setup.h"
#include <algorithm>

MouseHandler* gMouse_handler = nullptr;

struct Compare
{
    bool operator()(Clickable *a, Clickable *b) const
    {
        return a->get_click_layer() > b->get_click_layer();		//this will sort reverse, so biggest numbers come first -> descending
    }
};

MouseHandler::MouseHandler() : mCurrent_mouse_position{0, 0}
{
    mItem_on_mouse = nullptr;
    mSorted = false;
}

void MouseHandler::add_clickable(Clickable* c)
{
    if (c != nullptr) 
        this->mClickables_to_add.push_back(c);
}

void MouseHandler::del_clickable(Clickable* c)
{
    for (auto it = mClickables.begin(); it != mClickables.end(); ++it)
    {
        if (*it == c)
        {
            mClickables.erase(it);
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
    //set the mouse position for this tick
    this->set_mouse_position(mouse_x, mouse_y);
    this->get_mouse_position(&mouse_x, &mouse_y);

    //add potentially new clickables now
    for (auto clickable : mClickables_to_add)
    {
        mClickables.push_back(clickable);
        mSorted = false;
    }
    mClickables_to_add.clear();

    //sort again
    this->sort_clickables();

    auto const state = SDL_GetMouseState(nullptr, nullptr);
    const auto end = this->mClickables.end();


    for (unsigned i = 0; i < mClickables.size(); ++i)  // NOLINT(modernize-loop-convert)
    {
        auto it = mClickables.at(i);
        if (it == nullptr) continue;

        const auto rect = it->get_clickable_space();
        if (mouse_x > rect.x && mouse_x < rect.x + rect.w && 
            mouse_y > rect.y && mouse_y < rect.y + rect.h && 
            it->is_enabled())
        {
            it->on_mouse_over(mouse_x, mouse_y);				
            
            //get current state of mouse button to pass through to the clickable
            if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
                it->set_state(MOUSE_DOWN_LEFT);
            else if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
                it->set_state(MOUSE_DOWN_RIGHT);
            else if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
                it->set_state(MOUSE_DOWN_MIDDLE);
            else
                it->set_state(MOUSE_OVER);
        } 
        else 
        {
            it->set_state(MOUSE_OUT);
        }
    }
}

void MouseHandler::handle_event(SDL_Event *e)
{
    auto end = this->mClickables.end();
    const auto x = e->button.x;
    const auto y = e->button.y;

    auto clicked_on_something = false;	//may only click on one thing with each event.

    //when right clicking always set item to null
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == 3)
    {
        set_item_on_mouse(nullptr);
    }
    
    for (std::size_t i = 0; i < mClickables.size(); ++i)  // NOLINT(modernize-loop-convert)
    {
        auto it = mClickables.at(i);
        
        const auto rect = it->get_clickable_space();
        if (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h
            && it->is_enabled())
        {
            auto const state = SDL_GetMouseState(nullptr, nullptr);

            switch (e->type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (e->button.button == 1 && !clicked_on_something) {
                    it->set_state(MOUSE_DOWN_LEFT);
                    it->set_clicked(true);
                    it->on_click(x, y);
                    clicked_on_something = true;
                }
                else if (e->button.button == 2)
                {
                    it->set_state(MOUSE_DOWN_MIDDLE);
                    it->on_middle_click(x, y);
                }
                else if (e->button.button == 3)
                {
                    it->set_state(MOUSE_DOWN_RIGHT);
                    it->on_right_click(x, y);
                }
                break;
            case SDL_MOUSEMOTION:

                //get current state of mouse button to pass through to the clickable
                if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
                    it->set_state(MOUSE_DOWN_LEFT);
                else if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
                    it->set_state(MOUSE_DOWN_RIGHT);
                else if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
                    it->set_state(MOUSE_DOWN_MIDDLE);
                else
                    it->set_state(MOUSE_OVER);
                it->on_mouse_over(x, y);
                break;
            default:
                break;
            }
        }
        else //if the click wasn't inside the object set state and set clicked
        {
            it->set_state(MOUSE_OUT); 
            it->on_mouse_out(x, y);
            if (e->button.button == 1 && e->type == SDL_MOUSEBUTTONDOWN)
            {
                it->set_clicked(false);
            }
        }
    }
}

MouseItem* MouseHandler::get_item_on_mouse() const
{
    return mItem_on_mouse;
}

void MouseHandler::set_item_on_mouse(MouseItem* item)
{
    delete mItem_on_mouse;
    mItem_on_mouse = item;
}

void MouseHandler::set_mouse_position(const int x, const int y)
{
    //convert to 1920x1080 from the actual resolution
    this->mCurrent_mouse_position.x = static_cast<int>(double(x) / *gActual_screen_width * LOGICAL_SCREEN_WIDTH);
    this->mCurrent_mouse_position.y = static_cast<int>(double(y) / *gActual_screen_height * LOGICAL_SCREEN_HEIGHT);
}

void MouseHandler::get_mouse_position(int *x, int *y) const
{
    *x = this->mCurrent_mouse_position.x;
    *y = this->mCurrent_mouse_position.y;
}

bool MouseHandler::is_sorted() const
{
    return mSorted;
}

void MouseHandler::set_sorted(const bool sorted)
{
    mSorted = sorted;
}

void MouseHandler::sort_clickables()
{
    if (!mSorted)
    {
        std::sort(mClickables.begin(), mClickables.end(), Compare());
        mSorted = true;
    }
}
