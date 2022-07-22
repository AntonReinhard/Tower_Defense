#include "Timer.h"
#include <SDL.h>

Timer::Timer()
{
    mStart_ticks = 0;
    mPaused_ticks = 0;

    mPaused = false;
    mStarted = false;
}

void Timer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get current clock time
    mStart_ticks = SDL_GetTicks();
    mPaused_ticks = 0;
}

void Timer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStart_ticks = 0;
    mPaused_ticks = 0;
}

void Timer::pause()
{
    if (mStarted && !mPaused)
    {
        //Pause timer
        mPaused = true;

        //calc paused ticks
        mPaused_ticks = SDL_GetTicks() - mStart_ticks;
        mStart_ticks = 0;
    }
}

void Timer::unpause()
{
    if (mStarted && mPaused)
    {
        //Unpause timer
        mPaused = false;

        //Reset the starting ticks
        mStart_ticks = SDL_GetTicks() - mPaused_ticks;

        //Reset the paused ticks
        mPaused_ticks = 0;
    }
}

Uint32 Timer::get_ticks() const
{
    //The actual timer time
    Uint32 time = 0;

    if (mStarted)
    {
        if (mPaused)
        {
            //return number of ticks when timer was paused
            time = mPaused_ticks;
        }
        else
        {
            //return current time minus start time
            time = SDL_GetTicks() - mStart_ticks;
        }
    }

    return time;
}

bool Timer::is_started() const
{
    return mStarted;
}

bool Timer::is_paused() const
{
    return mPaused && mStarted;
}
