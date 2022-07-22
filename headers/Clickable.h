#pragma once
#include <SDL.h>

#include "Constants.h"

class Clickable
{
public:
    Clickable(LAYERS click_layer);
    virtual ~Clickable();

    //gets called when the object is clicked
    virtual void on_click(int mouse_x, int mouse_y) {};

    //gets called when the mouse hovers over the object. It gets called every tick this happens
    virtual void on_mouse_over(int mouse_x, int mouse_y) {};
    
    //gets called when the object is clicked with the right mouse button
    virtual void on_right_click(int mouse_x, int mouse_y) {};

    //gets called when the object is clicked with the middle mouse button
    virtual void on_middle_click(int mouse_x, int mouse_y) {};

    //gets called when the mouse isnt over the object
    virtual void on_mouse_out(int mouse_x, int mouse_y) {};

    SDL_Rect get_clickable_space() const;
    void set_clickable_space(SDL_Rect clickable_space);

    L_CLICKABLE_STATE get_state() const;
    void set_state(L_CLICKABLE_STATE state);

    bool is_clicked() const;
    virtual void set_clicked(bool value);

    void set_click_layer(LAYERS layer);
    LAYERS get_click_layer() const;

    //enables the object to be clicked
    void enable();

    //disables the object to be clicked; on_click functions won't be called anymore
    void disable();

    //returns if this object is currently clickable
    bool is_enabled() const;

private:
    //the state that the clickable is in right now. Is handled by the MouseHandler
    L_CLICKABLE_STATE mState;

    // true, if clickable was clicked and shows additional information
    bool mClicked; 

    //only if this is true the functions will be called
    bool mEnabled;

protected:
    //determines the order of stuff on the screens in terms of which will be clicked
    //low = background, high = foreground
    LAYERS  mClick_layer;

    //the space that can be clicked on to trigger an event
    SDL_Rect mClickable_space;
};
