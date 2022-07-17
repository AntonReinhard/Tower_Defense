#pragma once
#include <vector>
#include "Clickable.h"
#include <SDL.h>
#include "MouseItem.h"

class LTexture;

class MouseHandler
{
public:
    MouseHandler();

    void add_clickable(Clickable* c);
    void del_clickable(Clickable* c);

    //handles the mouse_over events
    void update();
    
    //handles all click related events
    void handle_event(SDL_Event* e);

    MouseItem* get_item_on_mouse() const;
    void set_item_on_mouse(MouseItem* item);

    void set_mouse_position(int x, int y);
    void get_mouse_position(int *x, int *y) const;

    bool is_sorted() const;
    void set_sorted(bool sorted);

    void sort_clickables();

private:
    bool mSorted;

    std::vector<Clickable*> mClickables;
    std::vector<Clickable*> mClickables_to_add;

    MouseItem* mItem_on_mouse;

    SDL_Point mCurrent_mouse_position;
};

//holds pointers to all clickable objects
extern MouseHandler* gMouse_handler;
