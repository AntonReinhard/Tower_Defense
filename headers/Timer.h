#pragma once

#include <SDL_hints.h>

//The application time based timer 
class Timer 
{ 
public: 
    
    //Initializes variables 
    Timer(); 
    //The various clock actions 
    void start(); 
    void stop(); 
    void pause(); 
    void unpause(); 
    
    //Gets the timer's time 
    Uint32 get_ticks() const; 
    
    //Checks the status of the timer 
    bool is_started() const; 
    bool is_paused() const; 
private: 
    //The clock time when the timer started 
    Uint32 mStart_ticks; 
    
    //The ticks stored when the timer was paused 
    Uint32 mPaused_ticks; 
    
    //The timer status 
    bool mPaused; 
    bool mStarted; 
};
